#ifndef FILESYSTEM
#define FILESYSTEM

#include "IFile.h"

using Path = std::filesystem::path;
class AppContext;

class FileSystem {
 public:
  FileSystem(const char* argv);
  virtual ~FileSystem();

  bool SetWriteDirectory(const Path& path);
  Path GetWriteDirectory();
  Path GetWorkingDirectory();
  bool AddSearchDirectory(const Path& path);
  bool DirectoryExists(const Path& path);
  bool FileExists(const Path& path);
  bool CreateDirectory(const Path& path);

  FilePtr OpenWrite(const Path& path);
  FilePtr OpenRead(const Path& path);

  std::vector<Path> GetFilesInDirectory(const Path& directory);

  static Path MakePosix(const Path& path);

 private:
  Path m_writeDirectory, m_workingDirectory;
  std::vector<Path> m_searchDirectories;
};

#endif