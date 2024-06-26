// #include "Precomp.h"
#include "ShaderBinding.h"
#include "IBindingValue.h"
#include "TBindingValue.h"

#include <utility>
#include "application/AppContext.h"
#include "utility/Logger.h"

ShaderBinding::ShaderBinding() {
  m_bindingIndex = -1;
  m_openGLType = 0;
}

ShaderBinding::ShaderBinding(int32_t index, const std::string& name, GLEnum openGLType) {
  m_bindingIndex = index;
  m_name = name;
  m_openGLType = openGLType;
}

ShaderBinding::~ShaderBinding() {
  m_value = nullptr;
}

ShaderBinding::ShaderBinding(ShaderBinding&& other)
  : m_bindingIndex(other.m_bindingIndex)
  , m_openGLType(other.m_openGLType)
  , m_value(other.m_value)
  , m_name(std::move(other.m_name)) {
}

ShaderBinding& ShaderBinding::operator=(ShaderBinding&& other) {
  m_bindingIndex = other.m_bindingIndex;
  m_openGLType = other.m_openGLType;
  m_value = other.m_value;
  m_name = std::move(other.m_name);
  return *this;
}

/// REFACTOR: Might want to not recreate value each time, check for dynamic cast or sth?
void ShaderBinding::Set(float value) {
  if (m_value) {
    static_cast<TBindingValue<float>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<float>(value));
}

void ShaderBinding::Set(std::vector<float> value) {
  if (m_value) {
    static_cast<TBindingValue<std::vector<float>>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<std::vector<float>>(value));
}

void ShaderBinding::Set(int32_t value) {
  if (m_value) {
    static_cast<TBindingValue<int32_t>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<int32_t>(value));
}

void ShaderBinding::Set(glm::mat4 value) {
  if (m_value) {
    static_cast<TBindingValue<glm::mat4>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<glm::mat4>(value));
}

void ShaderBinding::Set(std::vector<glm::mat4> value) {
  if (m_value) {
    static_cast<TBindingValue<std::vector<glm::mat4>>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<std::vector<glm::mat4>>(value));
}

void ShaderBinding::Set(std::vector<glm::mat3x4> value) {
  if (m_value) {
    static_cast<TBindingValue<std::vector<glm::mat3x4>>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<std::vector<glm::mat3x4>>(value));
}

void ShaderBinding::Set(glm::mat3x3 value) {
  if (m_value) {
    static_cast<TBindingValue<glm::mat3x3>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<glm::mat3x3>(value));
}

void ShaderBinding::Set(glm::vec2 value) {
  if (m_value) {
    static_cast<TBindingValue<glm::vec2>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<glm::vec2>(value));
}

void ShaderBinding::Set(glm::vec3 value) {
  if (m_value) {
    static_cast<TBindingValue<glm::vec3>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<glm::vec3>(value));
}

void ShaderBinding::Set(glm::vec4 value) {
  if (m_value) {
    static_cast<TBindingValue<glm::vec4>*>(m_value.get())->Update(value);
    return;
  }
  m_value = share(new TBindingValue<glm::vec4>(value));
}

void ShaderBinding::Update() {
  if (m_value)
    m_value->Set(m_bindingIndex);
  else
    GetContext().GetLogger()->log(LOG_WARN, "UpdateBinding(): Binding '%s' has no value", m_name.c_str());
}

bool ShaderBinding::HasValue() {
  return m_value.get();
}

int32_t ShaderBinding::GetIndex() {
  return m_bindingIndex;
}

uint32_t ShaderBinding::GetOpenGLType() {
  return m_openGLType;
}

const std::string& ShaderBinding::GetName() {
  return m_name;
}

IBindingValuePtr ShaderBinding::GetBindingValue() {
  return m_value;
}
