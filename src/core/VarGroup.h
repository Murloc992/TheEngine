#ifndef VARGROUP_H
#define VARGROUP_H

class Var;
class VarGroup {
 public:
  static VarGroup Null;

 public:
  VarGroup();
  VarGroup(const char* groupName);
  VarGroup(const VarGroup& o);
  VarGroup& operator=(const VarGroup& other);
  virtual ~VarGroup();
  const char* Name() const;

  std::vector<Var>& Vars();
  const std::vector<Var>& Vars() const;
  std::vector<VarGroup>& Groups();
  const std::vector<VarGroup>& Groups() const;
  Var& GetVar(const char* name);
  VarGroup& AddGroup(const char* name);
  VarGroup& AddGroup(const VarGroup& vGroup);
  bool AddVar(const Var& variable);
  VarGroup& GetGroup(const char* name);

 private:
  char* m_name;
  std::vector<Var> m_vars;
  std::vector<VarGroup> m_groups;
};

typedef std::shared_ptr<VarGroup> VarGroupPtr;

#endif
