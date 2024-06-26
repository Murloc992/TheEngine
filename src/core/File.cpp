// #include "Precomp.h"

#include "File.h"

#include "PathTools.h"

File::File(const Path& file, EFileMode mode) {
  m_mode = mode;
  auto filePath = MakePosix(file).generic_string();
  auto file_cstr = filePath.c_str();

  if (mode & EFM_READ) {
    if (mode & EFM_APPEND) {
      m_fileHandle = PHYSFS_openAppend(file_cstr);

      if (m_fileHandle == nullptr) {
        printf("File open append failed: %s %s\n", PHYSFS_getLastError(), file_cstr);
      }
    } else {
      m_fileHandle = PHYSFS_openRead(file_cstr);

      if (m_fileHandle == nullptr) {
        printf("File open read failed: %s %s\n", PHYSFS_getLastError(), file_cstr);
      }
    }
  } else if (mode & EFM_WRITE) {
    m_fileHandle = PHYSFS_openWrite(file_cstr);

    if (m_fileHandle == nullptr) {
      printf("File open write failed: %s %s\n", PHYSFS_getLastError(), file_cstr);
    }

  } else
    m_fileHandle = nullptr;
}

File::~File() {
  if (m_fileHandle) {
    if (m_mode & EFM_WRITE)
      PHYSFS_flush(m_fileHandle);

    PHYSFS_close(m_fileHandle);
  }
}

bool File::IsOpen() {
  return m_fileHandle != nullptr;
}

EFileMode File::GetMode() {
  return m_mode;
}

uint32_t File::GetLength() {
  if (m_fileHandle) {
    return PHYSFS_fileLength(m_fileHandle);
  } else
    return 0;
}

ByteBufferPtr File::Read() {
  uint32_t length = GetLength();
  return Read(length);
}

ByteBufferPtr File::Read(uint32_t length) {
  if (m_fileHandle) {
    ByteBuffer* buffer = new ByteBuffer();

    buffer->resize(length);
    uint32_t bytesRead = PHYSFS_read(m_fileHandle, (char*)buffer->data(), length, 1) * length;
    buffer->resize(bytesRead);

    return share(buffer);
  } else {
    return std::shared_ptr<ByteBuffer>();
  }
}

ByteBufferPtr File::ReadText() {
  uint32_t length = GetLength();
  return ReadText(length);
}

ByteBufferPtr File::ReadText(uint32_t length) {
  if (m_fileHandle) {
    ByteBuffer* buffer = new ByteBuffer();

    buffer->resize(length + 1);
    uint32_t bytesRead = PHYSFS_read(m_fileHandle, (char*)buffer->data(), length, 1) * length;
    buffer->resize(bytesRead + 1);
    buffer->data()[bytesRead] = '\0';

    return share(buffer);
  } else {
    return std::shared_ptr<ByteBuffer>();
  }
}

uint32_t File::Write(void* buffer, uint32_t length) {
  if (m_fileHandle) {
    return PHYSFS_write(m_fileHandle, (char*)buffer, length, 1) * length;
  } else {
    return 0;
  }
}