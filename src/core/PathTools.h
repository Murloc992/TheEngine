#pragma once

#ifdef _WIN32

using Path = std::filesystem::path;

static std::string _WorkaroundVisualStudio(const Path& path) {
  auto pathString = std::filesystem::canonical(path).string();
  std::replace(pathString.begin(), pathString.end(), '/', '\\');
  return pathString;
}

static Path _MakePosix(const Path& path) {
  std::wstring pathWstr = std::wstring(path.c_str());
  std::string pathStr = std::string(pathWstr.begin(), pathWstr.end());
  std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
  return Path(pathStr);
}

#endif

#ifdef _WIN32
  #define MakePosix(path) _MakePosix(path)
  #define WorkaroundVisualStudio(path) _WorkaroundVisualStudio(path)
#else
  #define MakePosix(path) path
  #define WorkaroundVisualStudio(path) path
#endif
