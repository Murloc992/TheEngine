#ifndef FORWARD_DECL_H
#define FORWARD_DECL_H

#define DECLARE_SHARED_PTR(var) \
  class var;                    \
  typedef std::shared_ptr<var> var##Ptr;

#define DECLARE_UNIQUE_PTR(var) \
  class var;                    \
  typedef std::unique_ptr<var> var##Ptr;

#define DECLARE_WEAK_PTR(var) \
  class var;                  \
  typedef std::weak_ptr<var> var##Ptr;

namespace std {
template <class T>
class shared_ptr;
}

class AppContext;
class ApplicationSettingsManager;
class ApplicationWindow;
class Camera;
class FileSystem;
class GUIEnvironment;
class IBindingValue;
class IFile;
class Image;
class ImageLoader;
class InputHandler;
class Logger;
class Mesh;
class MeshLoader;
class OpenGLExtensionLoader;
class ResourceManager;
class Shader;
class ShaderLoader;
class Timer;
class VarGroup;
class Material;
class Texture;
class IRenderObject;
class RenderStateManager;

typedef std::shared_ptr<Camera> CameraPtr;
typedef std::shared_ptr<IFile> FilePtr;
typedef std::shared_ptr<IBindingValue> IBindingValuePtr;
typedef std::shared_ptr<Image> ImagePtr;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::shared_ptr<Timer> TimerPtr;
typedef std::shared_ptr<IRenderObject> IRenderObjectPtr;
typedef std::shared_ptr<VarGroup> VarGroupPtr;

typedef uint32_t GLEnum;

#ifdef __linux__
namespace std {
namespace filesystem {
namespace __cxx11 {
class path;
}
}  // namespace filesystem
}  // namespace std

using Path = std::filesystem::__cxx11::path;
#else
namespace std {
namespace filesystem {
class path;
}
}  // namespace std

using Path = std::filesystem::path;
#endif

#endif