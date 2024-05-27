#ifndef RENDERABLE_OBJECT_MANAGER_H
#define RENDERABLE_OBJECT_MANAGER_H

#include "ForwardDecl.h"

/*
Render object manager does not hold any list of render objects.
Because those objects already exist on user code most likelly in a std::vector, why duplicate and sync that?.
*/

class RenderaObjectManager {
 public:
  RenderaObjectManager();
  virtual ~RenderaObjectManager();

  void RenderObjects(const std::vector<IRenderObject*>& renderObjects);
};

#endif