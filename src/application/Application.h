#pragma once

#include "AppContext.h"
#include "ForwardDecl.h"
/// REFACTOR: remove these includes
#include "core/Var.h"
#include "core/VarGroup.h"
#include "core/VarJsonReader.h"

class ApplicationWindow;

class Application {
 public:
  Application(int32_t argc, const char** argv);
  virtual ~Application();

  virtual bool Init() = 0;
  virtual bool Update() = 0;
  virtual bool Exit() = 0;

  virtual void OnWindowResized(uint32_t w, uint32_t h) = 0;
  virtual void OnFramebufferResized(uint32_t w, uint32_t h) = 0;
  virtual void OnWindowClose() = 0;
  virtual std::string GetApplicationId() = 0;

  bool DestroyContext();

  /// Call this one func..
  bool InitSimple(const std::string& title);
  bool InitConsoleOnly();
  ///.. Or call these manually
  bool InitContextBasics();
  bool InitFileSystem();
  bool InitWindowAndOpenGL(const std::string& title);
  bool LoadConfig();

  [[deprecated]] VarGroup& GetEngineVars();

 protected:
  /// make argv, argc private and add get/set for them.
  int32_t m_argc;
  const char** m_argv;
};