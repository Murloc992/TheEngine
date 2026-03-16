#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "IBufferObject.h"

#define RegisterBufferObject(type, gltype, components)                                                                                    \
  template <>                                                                                                                             \
  uint32_t BufferObject<type>::GetDataType() {                                                                                            \
    return gltype;                                                                                                                        \
  }                                                                                                                                       \
  template <>                                                                                                                             \
  uint32_t BufferObject<type>::GetComponentCount() {                                                                                      \
    return components;                                                                                                                    \
  }                                                                                                                                       \
  template <>                                                                                                                             \
  void BufferObject<type>::Init() {                                                                                                       \
    glGenBuffers(1, &this->Id);                                                                                                           \
  }                                                                                                                                       \
  template <>                                                                                                                             \
  void BufferObject<type>::Bind() {                                                                                                       \
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);                                                                                              \
  }                                                                                                                                       \
  template <>                                                                                                                             \
  void BufferObject<type>::UploadOrphan(size_t size) {                                                                                    \
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);                                                                                              \
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(type), 0, this->UsageHint);                                                               \
  }                                                                                                                                       \
  template <>                                                                                                                             \
  void BufferObject<type>::Upload() {                                                                                                     \
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);                                                                                              \
    glBufferData(GL_ARRAY_BUFFER, GetSize() * sizeof(type), data.size() > 0 ? &data[0] : 0, this->UsageHint);                             \
  }                                                                                                                                       \
  template <>                                                                                                                             \
  void BufferObject<type>::UploadSubData(const std::vector<type>& subdata, uint32_t offset) {                                             \
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);                                                                                              \
    glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(type), subdata.size() * sizeof(type), &subdata[0]);                                  \
  }                                                                                                                                       \
  template <>                                                                                                                             \
  void BufferObject<type>::UploadOrphanSubData(size_t orphan_size, size_t data_size, const std::vector<type>& subdata, uint32_t offset) { \
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);                                                                                              \
    glBufferData(GL_ARRAY_BUFFER, orphan_size * sizeof(type), 0, this->UsageHint);                                                        \
    glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(type), data_size * sizeof(type), &subdata[0]);                                       \
  }

template <class T>
struct BufferObject : public IBufferObject {
  uint32_t preallocatedSize;
  std::vector<T> data;

  BufferObject(IBufferObject::USAGE_HINT hint, uint32_t prealloc = 0)
    : IBufferObject(hint)
    , preallocatedSize(prealloc) {
  }

  virtual ~BufferObject() {
    data.clear();
  }

  // void Init_buffers();
  virtual void Init();
  virtual void Bind();
  virtual void Upload();
  virtual void UploadOrphan(size_t size);
  virtual void UploadSubData(const std::vector<T>& subdata, uint32_t offset);
  virtual void UploadOrphanSubData(size_t orphan_size, size_t data_size, const std::vector<T>& subdata, uint32_t offset);
  virtual void Free() {
    std::vector<T>().swap(data);
  }
  virtual void Clear() {
    data.clear();
    // data.shrink_to_fit();
  }

  virtual IBufferObject::IBO_TYPE GetType() {
    return IBufferObject::DATA;
  }
  virtual uint32_t GetDataType();
  virtual uint32_t GetComponentCount();
  virtual uint32_t GetSize() {
    return (uint32_t)(data.size() > 0 ? data.size() : preallocatedSize);
    // return (uint32_t)(data.size() > 0 ? data.size() : (data.capacity() > 0 ? data.capacity() : preallocatedSize));
  }
};

#define RegisterUniformBufferObject(type, gltype, components)                                                                                    \
  template <>                                                                                                                                    \
  uint32_t UniformBufferObject<type>::GetDataType() {                                                                                            \
    return gltype;                                                                                                                               \
  }                                                                                                                                              \
  template <>                                                                                                                                    \
  uint32_t UniformBufferObject<type>::GetComponentCount() {                                                                                      \
    return components;                                                                                                                           \
  }                                                                                                                                              \
  template <>                                                                                                                                    \
  void UniformBufferObject<type>::Init() {                                                                                                       \
    glGenBuffers(1, &this->Id);                                                                                                                  \
  }                                                                                                                                              \
  template <>                                                                                                                                    \
  void UniformBufferObject<type>::Bind() {                                                                                                       \
    glBindBuffer(GL_UNIFORM_BUFFER, this->Id);                                                                                                   \
  }                                                                                                                                              \
  template <>                                                                                                                                    \
  void UniformBufferObject<type>::UploadOrphan(size_t size) {                                                                                    \
    glBindBuffer(GL_UNIFORM_BUFFER, this->Id);                                                                                                   \
    glBufferData(GL_UNIFORM_BUFFER, size * sizeof(type), 0, this->UsageHint);                                                                    \
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->Id);                                                                                            \
  }                                                                                                                                              \
  template <>                                                                                                                                    \
  void UniformBufferObject<type>::Upload() {                                                                                                     \
    glBindBuffer(GL_UNIFORM_BUFFER, this->Id);                                                                                                   \
    glBufferData(GL_UNIFORM_BUFFER, GetSize() * sizeof(type), data.size() > 0 ? &data[0] : 0, this->UsageHint);                                  \
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->Id);                                                                                            \
  }                                                                                                                                              \
  template <>                                                                                                                                    \
  void UniformBufferObject<type>::UploadSubData(const std::vector<type>& subdata, uint32_t offset) {                                             \
    glBindBuffer(GL_UNIFORM_BUFFER, this->Id);                                                                                                   \
    glBufferSubData(GL_UNIFORM_BUFFER, offset * sizeof(type), subdata.size() * sizeof(type), &subdata[0]);                                       \
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->Id);                                                                                            \
  }                                                                                                                                              \
  template <>                                                                                                                                    \
  void UniformBufferObject<type>::UploadOrphanSubData(size_t orphan_size, size_t data_size, const std::vector<type>& subdata, uint32_t offset) { \
    glBindBuffer(GL_UNIFORM_BUFFER, this->Id);                                                                                                   \
    glBufferData(GL_UNIFORM_BUFFER, orphan_size * sizeof(type), 0, this->UsageHint);                                                             \
    glBufferSubData(GL_UNIFORM_BUFFER, offset * sizeof(type), data_size * sizeof(type), &subdata[0]);                                            \
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->Id);                                                                                            \
  }

template <class T>
struct UniformBufferObject : public IBufferObject {
  uint32_t preallocatedSize;
  std::vector<T> data;

  UniformBufferObject(IBufferObject::USAGE_HINT hint, uint32_t prealloc = 0)
    : IBufferObject(hint)
    , preallocatedSize(prealloc) {
  }

  virtual ~UniformBufferObject() {
    data.clear();
  }

  // void Init_buffers();
  virtual void Init();
  virtual void Bind();
  virtual void Upload();
  virtual void UploadOrphan(size_t size);
  virtual void UploadSubData(const std::vector<T>& subdata, uint32_t offset);
  virtual void UploadOrphanSubData(size_t orphan_size, size_t data_size, const std::vector<T>& subdata, uint32_t offset);
  virtual void Free() {
    std::vector<T>().swap(data);
  }
  virtual void Clear() {
    data.clear();
    // data.shrink_to_fit();
  }

  virtual IBufferObject::IBO_TYPE GetType() {
    return IBufferObject::UNIFORM;
  }
  virtual uint32_t GetDataType();
  virtual uint32_t GetComponentCount();
  virtual uint32_t GetSize() {
    return (uint32_t)(data.size() > 0 ? data.size() : preallocatedSize);
    // return (uint32_t)(data.size() > 0 ? data.size() : (data.capacity() > 0 ? data.capacity() : preallocatedSize));
  }
};

#define RegisterIndexBufferObject(type, gltype, components)                                                                                    \
  template <>                                                                                                                                  \
  uint32_t IndexBufferObject<type>::GetDataType() {                                                                                            \
    return gltype;                                                                                                                             \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  uint32_t IndexBufferObject<type>::GetComponentCount() {                                                                                      \
    return components;                                                                                                                         \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::Init() {                                                                                                       \
    glGenBuffers(1, &this->Id);                                                                                                                \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::Bind() {                                                                                                       \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::UploadOrphan(size_t size) {                                                                                    \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(type), 0, this->UsageHint);                                                            \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::Upload() {                                                                                                     \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetSize() * sizeof(type), data.size() > 0 ? &data[0] : 0, this->UsageHint);                          \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::UploadSubData(const std::vector<type>& subdata, uint32_t offset) {                                             \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(type), subdata.size() * sizeof(type), &subdata[0]);                               \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::UploadOrphanSubData(size_t orphan_size, size_t data_size, const std::vector<type>& subdata, uint32_t offset) { \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, orphan_size * sizeof(type), 0, this->UsageHint);                                                     \
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(type), data_size * sizeof(type), &subdata[0]);                                    \
  }

#define RegisterIndexBufferObject(type, gltype, components)                                                                                    \
  template <>                                                                                                                                  \
  uint32_t IndexBufferObject<type>::GetDataType() {                                                                                            \
    return gltype;                                                                                                                             \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  uint32_t IndexBufferObject<type>::GetComponentCount() {                                                                                      \
    return components;                                                                                                                         \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::Init() {                                                                                                       \
    glGenBuffers(1, &this->Id);                                                                                                                \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::Bind() {                                                                                                       \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::UploadOrphan(size_t size) {                                                                                    \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(type), 0, this->UsageHint);                                                            \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::Upload() {                                                                                                     \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetSize() * sizeof(type), data.size() > 0 ? &data[0] : 0, this->UsageHint);                          \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::UploadSubData(const std::vector<type>& subdata, uint32_t offset) {                                             \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(type), subdata.size() * sizeof(type), &subdata[0]);                               \
  }                                                                                                                                            \
  template <>                                                                                                                                  \
  void IndexBufferObject<type>::UploadOrphanSubData(size_t orphan_size, size_t data_size, const std::vector<type>& subdata, uint32_t offset) { \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);                                                                                           \
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, orphan_size * sizeof(type), 0, this->UsageHint);                                                     \
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(type), data_size * sizeof(type), &subdata[0]);                                    \
  }

template <class T>
struct IndexBufferObject : public IBufferObject {
  uint32_t preallocatedSize;
  std::vector<T> data;

  IndexBufferObject(IBufferObject::USAGE_HINT hint, uint32_t prealloc = 0)
    : IBufferObject(hint)
    , preallocatedSize(prealloc) {
  }

  virtual ~IndexBufferObject() {
    data.clear();
  }

  // void Init_buffers();
  virtual void Init();
  virtual void Bind();
  virtual void Upload();
  virtual void UploadOrphan(size_t size);
  virtual void UploadSubData(const std::vector<T>& subdata, uint32_t offset);
  virtual void UploadOrphanSubData(size_t orphan_size, size_t data_size, const std::vector<T>& subdata, uint32_t offset);
  virtual void Free() {
    std::vector<T>().swap(data);
  }
  virtual void Clear() {
    data.clear();
    // data.shrink_to_fit();
  }

  virtual IBufferObject::IBO_TYPE GetType() {
    return IBufferObject::INDEX;
  }
  virtual uint32_t GetDataType();
  virtual uint32_t GetComponentCount();
  virtual uint32_t GetSize() {
    return (uint32_t)(data.size() > 0 ? data.size() : preallocatedSize);
    // return (uint32_t)(data.size() > 0 ? data.size() : (data.capacity() > 0 ? data.capacity() : preallocatedSize));
  }
};

#endif  // BUFFER_OBJECT_H
