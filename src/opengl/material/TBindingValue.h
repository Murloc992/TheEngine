#ifndef TBINDING_VALUE
#define TBINDING_VALUE

#include "ForwardDecl.h"
#include "IBindingValue.h"

template <class T>
class TBindingValue : public IBindingValue {
 public:
  TBindingValue(T value);
  virtual ~TBindingValue();
  virtual void Set(int32_t index);
  virtual void Update(T value) {
    m_value = value;
  }

 private:
  T m_value;
};

template <>
void TBindingValue<float>::Set(int32_t index);
template <>
void TBindingValue<std::vector<float>>::Set(int32_t index);
template <>
void TBindingValue<int32_t>::Set(int32_t index);
template <>
void TBindingValue<glm::mat4>::Set(int32_t index);
template <>
void TBindingValue<std::vector<glm::mat4>>::Set(int32_t index);
template <>
void TBindingValue<std::vector<glm::mat3x4>>::Set(int32_t index);
template <>
void TBindingValue<glm::mat3x3>::Set(int32_t index);
template <>
void TBindingValue<glm::vec3>::Set(int32_t index);
template <>
void TBindingValue<glm::vec4>::Set(int32_t index);

template <class T>
TBindingValue<T>::TBindingValue(T value) {
  m_value = value;
}

template <class T>
TBindingValue<T>::~TBindingValue() {
}

#endif