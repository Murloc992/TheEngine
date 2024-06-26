// #include "Precomp.h"
#include "ResourceManager.h"
#include "ImageLoader.h"
#include "MeshLoader.h"
#include "ShaderLoader.h"

ResourceManager::ResourceManager() {
  m_shaderLoader = new ShaderLoader();
  m_imageLoader = new ImageLoader();
  m_meshLoader = new MeshLoader();
}

ResourceManager::~ResourceManager() {
  delete m_shaderLoader;
  delete m_meshLoader;
  delete m_imageLoader;
}

ImagePtr ResourceManager::LoadImage(const Path& filePath) {
  return GetImageLoader()->Load(filePath);
}

MeshPtr ResourceManager::LoadMesh(const Path& filePath) {
  return GetMeshLoader()->Load(filePath);
}

ShaderPtr ResourceManager::LoadShader(const Path& shaderPath, bool reload) {
  return GetShaderLoader()->Load(shaderPath, reload);
}

ShaderPtr ResourceManager::LoadShader(const Path& vertexPath, const Path& fragmentPath, bool reload) {
  return GetShaderLoader()->Load(vertexPath, fragmentPath, reload);
}

ImageLoader* ResourceManager::GetImageLoader() {
  return m_imageLoader;
}

ShaderLoader* ResourceManager::GetShaderLoader() {
  return m_shaderLoader;
}

MeshLoader* ResourceManager::GetMeshLoader() {
  return m_meshLoader;
}