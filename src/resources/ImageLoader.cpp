// #include "Precomp.h"

#include "ImageLoader.h"
#include "PNGLoader.h"
#include "TGAloader.h"
#include "opengl/Texture.h"

#include "core/FileSystem.h"
#include "utility/Logger.h"

ImageLoader::ImageLoader() {
  AddLoader(new png_loader());
  AddLoader(new tgaloader());
}

ImageLoader::~ImageLoader() {
  for (IImageLoader* l : m_loaders)
    delete l;
}

void ImageLoader::AddLoader(IImageLoader* loader) {
  auto it = std::find_if(m_loaders.begin(), m_loaders.end(), [&loader](IImageLoader* l) {
    return l == loader;
  });

  if (it == m_loaders.end())
    m_loaders.push_back(loader);
}

ImagePtr ImageLoader::Load(const Path& filePath) {
  auto res = this->GetResource(filePath);
  std::string filename = filePath.filename().generic_string();
  if (res.resource) {
    GetContext().GetLogger()->log(LOG_INFO, "Found image '%s' in cache, skipping loading.", filename.c_str());
    return res.resource;
  }
  std::string ext = filePath.extension().generic_string();
  GetContext().GetLogger()->log(LOG_INFO, "Image load. Name: '%s' extension: '%s'", filename.c_str(), ext.c_str());
  auto fileSystem = GetContext().GetFileSystem();

  if (fileSystem->OpenRead(filePath)) {
    for (IImageLoader* l : m_loaders) {
      if (l->CheckByExtension(ext)) {
        FilePtr file = fileSystem->OpenRead(filePath);

        if (file) {
          ByteBufferPtr buffer = file->Read();

          if (buffer->size() != 0) {
            GetContext().GetLogger()->log(LOG_INFO, "Image file size: %u", buffer->size());

            res.path = filePath;
            res.resource = l->Load(buffer->data(), buffer->size());
            this->AddResource(res);
            return res.resource;
          }
        }
      }
    }
  }

  return nullptr;
}