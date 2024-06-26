#ifndef IFILE_H
#define IFILE_H

#include "EFileMode.h"

typedef std::vector<uint8_t> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

class IFile {
 public:
  virtual ~IFile() {};
  virtual bool IsOpen() = 0;
  virtual EFileMode GetMode() = 0;
  virtual uint32_t GetLength() = 0;
  virtual ByteBufferPtr Read() = 0;
  virtual ByteBufferPtr Read(uint32_t length) = 0;
  /// Reads a null terminated buffer
  virtual ByteBufferPtr ReadText() = 0;
  virtual ByteBufferPtr ReadText(uint32_t length) = 0;
  virtual uint32_t Write(void* buffer, uint32_t length) = 0;
};

typedef std::shared_ptr<IFile> FilePtr;

#endif