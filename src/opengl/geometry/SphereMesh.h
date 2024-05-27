#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "opengl/BufferObject.h"
#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"

class SphereMesh : public Mesh {
 private:
  float m_radius;
  glm::tvec4<uint8_t> m_color;

 public:
  SphereMesh(float radius = 1.0f, const int32_t stack_count = 18, const int32_t sector_count = 36, bool flipped_normals = false,
             glm::tvec4<uint8_t> color = glm::tvec4<uint8_t>(1));

  virtual ~SphereMesh() {};

  // void Render(bool wireframe = false, bool ccw_winding = true)
  //{
  //	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //	if (!ccw_winding) glFrontFace(GL_CW);
  //	Render();
  //	if (!ccw_winding) glFrontFace(GL_CCW);
  //	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // }
};

#endif  // SPHEREMESH_H
