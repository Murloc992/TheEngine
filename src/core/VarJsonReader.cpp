// #include "Precomp.h"
#include "VarJsonReader.h"
#include "application/AppContext.h"
#include "core/FileSystem.h"
#include "core/Var.h"
#include "core/VarGroup.h"
#include "json/json.h"
#include "json/reader.h"
#include "utility/Logger.h"

VarJsonReader::VarJsonReader() {
  m_logger = GetContext().GetLogger();
  m_fileSystem = GetContext().GetFileSystem();
}

VarJsonReader::~VarJsonReader() {
}

void LoadGroup(VarGroup& g, Json::Value& jg, Logger* logger);
bool VarJsonReader::Read(void* buffer, uint32_t size, VarGroup& group) {
  Json::Reader reader;
  const char* doc = (const char*)buffer;

  Json::Value root;

  if (!reader.parse(doc, doc + size, root) && m_logger)
    m_logger->log(LOG_WARN, "%s\n", "Failed to parse json buffer.");

  LoadGroup(group, root, m_logger);
  return true;
}

bool VarJsonReader::Read(const Path& fileName, VarGroup& group) {
  FilePtr file = m_fileSystem->OpenRead(fileName);

  if (file && file->IsOpen()) {
    ByteBufferPtr buffer = file->Read();
    return Read(buffer->data(), buffer->size(), group);
  }

  return false;
}

void BuildGroup(VarGroup& group, Json::Value& parentValue);
void WriteFile(const Path& fileName, const std::string& jsonString);

bool VarJsonReader::Write(const Path& fileName, VarGroup& group) {
  Json::StyledWriter writer;

  Json::Value root = Json::Value(Json::objectValue);

  BuildGroup(group, root);

  std::string buf = writer.write(root);

  WriteFile(fileName, buf);

  return true;
}

void WriteFile(const Path& fileName, const std::string& jsonString) {
  FilePtr file = GetContext().GetFileSystem()->OpenWrite(fileName);

  if (file && file->IsOpen()) {
    file->Write((void*)&jsonString[0], jsonString.size());
  }
}

void BuildVars(VarGroup& group, Json::Value& groupValue);
void BuildGroup(VarGroup& group, Json::Value& parentValue) {
  parentValue[group.Name()] = Json::Value(Json::objectValue);
  Json::Value& newGroup = parentValue[group.Name()];

  BuildVars(group, newGroup);

  for (auto& varGroup : group.Groups()) {
    BuildGroup(varGroup, newGroup);
  }
}

void BuildVars(VarGroup& group, Json::Value& groupValue) {
  for (auto& var : group.Vars()) {
    switch (var.Type()) {
      case VARB:
        groupValue[var.Name()] = Json::Value(var.ValueB());
        break;

      case VARI:
        groupValue[var.Name()] = Json::Value(var.ValueI());
        break;

      case VARF:
        groupValue[var.Name()] = Json::Value(var.ValueF());
        break;

      case VARS:
        groupValue[var.Name()] = Json::Value(var.ValueS());
        break;

      default:
        break;
    }
  }
}

void LoadGroup(VarGroup& g, Json::Value& jg, Logger* logger) {
  Json::Value& jgroup = jg[g.Name()];

  if (jgroup.isNull() || !jgroup.isObject())
    return;

  if (logger)
    logger->log(LOG_INFO, "Loading group: %s", g.Name());

  for (Var& var : g.Vars()) {
    Json::Value& jvar = jgroup[var.Name()];

    if (!jvar.isNull())
      switch (var.Type()) {
        case VARB:
          var.Value(jvar.asBool());
          break;
        case VARI:
          var.Value(jvar.asInt());
          break;
        case VARF:
          var.Value(jvar.asFloat());
          break;
        case VARS:
          var.Value(jvar.asString().c_str());
          break;
        default:
          break;
      }
  }

  for (VarGroup& group : g.Groups())
    LoadGroup(group, jgroup, logger);
}