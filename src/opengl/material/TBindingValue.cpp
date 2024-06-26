// #include "Precomp.h"
#include "TBindingValue.h"
#include "IBindingValue.h"

template <>
void TBindingValue<int32_t>::Set(int32_t index) {
  glUniform1i(index, m_value);
}

template <>
void TBindingValue<float>::Set(int32_t index) {
  glUniform1f(index, m_value);
}

template <>
void TBindingValue<std::vector<float>>::Set(int32_t index) {
  glUniform1fv(index, m_value.size(), &m_value[0]);
}

template <>
void TBindingValue<glm::mat4>::Set(int32_t index) {
  glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(m_value));
}

template <>
void TBindingValue<std::vector<glm::mat4>>::Set(int32_t index) {
  glUniformMatrix4fv(index, m_value.size(), GL_FALSE, glm::value_ptr(m_value[0]));
}

template <>
void TBindingValue<std::vector<glm::mat3x4>>::Set(int32_t index) {
  glUniformMatrix3x4fv(index, m_value.size(), GL_FALSE, glm::value_ptr(m_value[0]));
}

template <>
void TBindingValue<glm::mat3x3>::Set(int32_t index) {
  glUniformMatrix3fv(index, 1, GL_FALSE, glm::value_ptr(m_value));
}

template <>
void TBindingValue<glm::vec2>::Set(int32_t index) {
  glUniform2fv(index, 1, glm::value_ptr(m_value));
}

template <>
void TBindingValue<glm::vec3>::Set(int32_t index) {
  glUniform3fv(index, 1, glm::value_ptr(m_value));
}

template <>
void TBindingValue<glm::vec4>::Set(int32_t index) {
  glUniform4fv(index, 1, glm::value_ptr(m_value));
}