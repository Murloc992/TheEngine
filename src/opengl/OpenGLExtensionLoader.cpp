// #include "Precomp.h"
#include "OpenGLExtensionLoader.h"
#include "application/AppContext.h"
#include "utility/Logger.h"

#include <glad/glad.h>

OpenGLExtensionLoader::OpenGLExtensionLoader() {
}

OpenGLExtensionLoader::~OpenGLExtensionLoader() {
}

bool OpenGLExtensionLoader::load_extensions() {
  return gladLoadGL();
}

// TODO (serengeor#1#): log this
bool OpenGLExtensionLoader::check_and_output_errors() {
  uint32_t err = GL_NO_ERROR;

  bool ret = false;

  while ((err = glGetError()) != GL_NO_ERROR) {
    GetContext().GetLogger()->log(LOG_ERROR, "GL_ERROR: %s", gl_error_to_string(err).c_str());
    ret = true;
  }

  return ret;
}

std::string OpenGLExtensionLoader::gl_error_to_string(uint32_t error) {
  switch (error) {
    case 0:
      return "GL_NO_ERROR";
    case 0x0500:
      return "GL_INVALID_ENUM";
    case 0x0501:
      return "GL_INVALID_VALUE";
    case 0x0502:
      return "GL_INVALID_OPERATION";
    case 0x0503:
      return "GL_STACK_OVERFLOW";
    case 0x0504:
      return "GL_STACK_UNDERFLOW";
    case 0x0505:
      return "GL_OUT_OF_MEMORY";
    case 0x0506:
      return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:
      return "BAD ERROR CODE";
  }
}

std::string OpenGLExtensionLoader::gl_fbo_error_to_string(uint32_t error) {
  std::string r;
  switch (error) {
    case 0x8CD5:
      r = "GL_FRAMEBUFFER_COMPLETE";
      break;
    case 0x8CD6:
      r = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
      break;
    case 0x8CD7:
      r = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
      break;
    case 0x8CDB:
      r = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
      break;
    case 0x8CDC:
      r = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
      break;
    case 0x8CDD:
      r = "GL_FRAMEBUFFER_UNSUPPORTED";
      break;
    case 0x8D56:
      r = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
      break;
    case 0x8DA8:
      r = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
      break;
    case 0x8219:
      r = "GL_FRAMEBUFFER_UNDEFINED";
      break;
    default:
      r = "GL_BAD_ERROR_CODE: " + error;  // fix this
      break;
  }
  r += __FILE__;
  r += __LINE__;
  return r;
}