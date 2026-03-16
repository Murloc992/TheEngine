#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include "ForwardDecl.h"

class AppContext;
AppContext& GetContext();

#define LOG(level, formatString, ...) GetContext().GetLogger()->log(level, formatString, ##__VA_ARGS__)
#define LOG_INFO(formatString, ...) LOG(LOG_INFO, formatString, ##__VA_ARGS__)
#define LOG_DEBUG(formatString, ...) LOG(LOG_DEBUG, formatString, ##__VA_ARGS__)
#define LOG_WARN(formatString, ...) LOG(LOG_WARN, formatString, ##__VA_ARGS__)
#define LOG_ERROR(formatString, ...) LOG(LOG_ERROR, formatString, ##__VA_ARGS__)
#define LOG_CRITICAL(formatString, ...) LOG(LOG_CRITICAL, formatString, ##__VA_ARGS__)


class AppContext {
 protected:
  AppContext();
  virtual ~AppContext();

 public:
  friend class Application;
  static AppContext& Instance();
  bool IsInitialized();
  bool IsInitializedNoGL();

  ApplicationSettingsManager* GetApplicationSettingsManager();
  ApplicationWindow* GetWindow();
  FileSystem* GetFileSystem();
  GUIEnvironment* GetGUIEnvironment();
  InputHandler* GetInputHandler();
  Logger* GetLogger();
  OpenGLExtensionLoader* GetOpenGLExtensionLoader();
  ResourceManager* GetResourceManager();
  TimerPtr GetTimer();
  TimerPtr GetProfileTimer();

  void SetInputHandler(InputHandler* inputHandler);
  void SetGUIEnvironment(GUIEnvironment* guiEnvironment);

 protected:
  ApplicationSettingsManager* p_settingsManager;
  ApplicationWindow* p_window;
  FileSystem* p_fileSystem;
  GUIEnvironment* p_guiEnv;
  InputHandler* p_inputHandler;
  Logger* p_logger;
  OpenGLExtensionLoader* p_openGLExtensionLoader;
  ResourceManager* p_resourceManager;
  TimerPtr p_timer;
  TimerPtr profile_timer;
};

#endif
