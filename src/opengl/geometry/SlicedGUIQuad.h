#pragma once

#include "opengl/BufferObject.h"
#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"
#include "utility/Rect2d.h"
class SlicedGUIQuad {
 private:
  glm::vec2 m_size;
  glm::vec2 ratio;
  float m_margin;

  std::vector<glm::vec3> CreateVerts() {
    std::vector<glm::vec3> verts;
    verts.resize(16);

    /// LL
    verts[0] = glm::vec3(-m_size.x, -m_size.y, 0);

    verts[1] = glm::vec3(-m_size.x + m_margin * ratio.x, -m_size.y, 0);                       /// X+
    verts[2] = glm::vec3(-m_size.x + m_margin * ratio.x, -m_size.y + m_margin * ratio.y, 0);  /// Y+ X+
    verts[3] = glm::vec3(-m_size.x, -m_size.y + m_margin * ratio.y, 0);                       /// Y+
    /// ///////////////////////////////////////////////////////

    /// LR
    verts[4] = glm::vec3(m_size.x, -m_size.y, 0);

    verts[5] = glm::vec3(m_size.x - m_margin * ratio.x, -m_size.y, 0);                       /// X- Y
    verts[6] = glm::vec3(m_size.x - m_margin * ratio.x, -m_size.y + m_margin * ratio.y, 0);  /// X- Y+
    verts[7] = glm::vec3(m_size.x, -m_size.y + m_margin * ratio.y, 0);                       /// X Y+
    /// ///////////////////////////////////////////////////////

    /// UR
    verts[8] = glm::vec3(m_size.x, m_size.y, 0);

    verts[9] = glm::vec3(m_size.x - m_margin * ratio.x, m_size.y, 0);                        /// X- Y
    verts[10] = glm::vec3(m_size.x - m_margin * ratio.x, m_size.y - m_margin * ratio.y, 0);  /// X- Y-
    verts[11] = glm::vec3(m_size.x, m_size.y - m_margin * ratio.y, 0);                       /// X Y-
    /// ///////////////////////////////////////////////////////

    /// UL
    verts[12] = glm::vec3(-m_size.x, m_size.y, 0);

    verts[13] = glm::vec3(-m_size.x + m_margin * ratio.x, m_size.y, 0);
    verts[14] = glm::vec3(-m_size.x, m_size.y - m_margin * ratio.y, 0);
    verts[15] = glm::vec3(-m_size.x + m_margin * ratio.x, m_size.y - m_margin * ratio.y, 0);
    /// ///////////////////////////////////////////////////////
    ///
    return verts;
  }

  void CreateInds() {
    const static uint32_t ind_vec[] {1, 3,  0,  1, 2,  3, 5, 2,  1,  5,  6,  2,  4,  6,  5,  4,  7,  6,  2,  14, 3,  2,  15, 14, 6,  15, 2,
                                     6, 10, 15, 7, 10, 6, 7, 11, 10, 15, 12, 14, 15, 13, 12, 10, 13, 15, 10, 9,  13, 11, 9,  10, 11, 8,  9};
    indices->data.insert(indices->data.begin(), ind_vec, ind_vec + 54);
  }

 public:
  BufferObject<glm::vec3>* pos;
  BufferObject<glm::vec2>* tex_coords;
  IndexBufferObject<uint32_t>* indices;

  std::shared_ptr<Mesh> glmesh;

  static Shader quad_shader_textured;

  SlicedGUIQuad(glm::vec2 size = glm::vec2(1), float margin = 1.f) {
    float minRatio = std::min(size.x, size.y) / std::max(size.x, size.y);
    if (size.x > size.y) {
      this->m_size.x = 1.f;
      this->m_size.y = minRatio;
    } else if (size.x < size.y) {
      this->m_size.x = minRatio;
      this->m_size.y = 1.f;
    } else {
      this->m_size.x = 1.f;
      this->m_size.y = 1.f;
    }

    this->m_margin = margin;
  }

  std::vector<glm::vec2> CreateTCoords(const std::vector<glm::vec2>& uvs) {
    std::vector<glm::vec2> coords;
    coords.resize(16);

    glm::vec2 tm(1);
    tm.x = 1.f / 256.f * m_margin;
    tm.y = 1.f / 256.f * m_margin;

    glm::vec2 a = glm::vec2(helpers::limit(uvs[0].x, 0.f, 1.f), helpers::limit(uvs[0].y, 0.f, 1.f));
    glm::vec2 b = glm::vec2(helpers::limit(uvs[1].x, 0.f, 1.f), helpers::limit(uvs[1].y, 0.f, 1.f));
    glm::vec2 c = glm::vec2(helpers::limit(uvs[2].x, 0.f, 1.f), helpers::limit(uvs[2].y, 0.f, 1.f));
    glm::vec2 d = glm::vec2(helpers::limit(uvs[3].x, 0.f, 1.f), helpers::limit(uvs[3].y, 0.f, 1.f));

    coords[0] = glm::vec2(a.x, a.y);
    coords[1] = glm::vec2(a.x + tm.x, a.y);
    coords[2] = glm::vec2(a.x + tm.x, a.y - tm.y);
    coords[3] = glm::vec2(a.x, a.y - tm.y);

    coords[4] = glm::vec2(b.x, b.y);
    coords[5] = glm::vec2(b.x - tm.x, b.y);
    coords[6] = glm::vec2(b.x - tm.x, b.y - tm.y);
    coords[7] = glm::vec2(b.x, b.y - tm.y);

    coords[8] = glm::vec2(c.x, c.y);
    coords[9] = glm::vec2(c.x - tm.x, c.y);
    coords[10] = glm::vec2(c.x - tm.x, c.y + tm.y);
    coords[11] = glm::vec2(c.x, c.y + tm.y);

    coords[12] = glm::vec2(d.x, d.y);
    coords[13] = glm::vec2(d.x + tm.x, d.y);
    coords[14] = glm::vec2(d.x, d.y + tm.y);
    coords[15] = glm::vec2(d.x + tm.x, d.y + tm.y);

    return coords;
  }

  virtual ~SlicedGUIQuad() {};

  virtual bool Init() {
    glmesh = share(new Mesh());

    pos = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::DYNAMIC);
    pos->Init();
    pos->data.resize(16);
    pos->UploadOrphan(16);
    tex_coords = new BufferObject<glm::vec2>(IBufferObject::USAGE_HINT::DYNAMIC);
    tex_coords->Init();
    tex_coords->data.resize(16);
    tex_coords->UploadOrphan(16);
    indices = new IndexBufferObject<uint32_t>(IBufferObject::USAGE_HINT::STATIC);

    glmesh->buffers[Mesh::POSITION] = pos;
    glmesh->buffers[Mesh::TEXCOORD] = tex_coords;
    glmesh->buffers[Mesh::INDICES] = indices;

    CreateVerts();
    std::vector<glm::vec2> defuv;
    defuv.push_back(glm::vec2(0, 1));
    defuv.push_back(glm::vec2(1, 1));
    defuv.push_back(glm::vec2(1, 0));
    defuv.push_back(glm::vec2(0, 0));
    CreateTCoords(defuv);
    CreateInds();

    glmesh->Init();
    return true;
  }

  void Render() {
    //        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    //        glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(glmesh->vao);
    glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);
    //         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }

  void SetTCoords(const std::vector<glm::vec2>& tcoords) {
    tex_coords->UploadSubData(CreateTCoords(tcoords), 0);
  }

  void SetRatio(const glm::vec2& size, const int& margin = 1) {
    m_margin = (float)margin;

    ratio.x = 1.f / size.x;
    ratio.y = 1.f / size.y;

    pos->UploadSubData(CreateVerts(), 0);
  }
};
