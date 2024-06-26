// #include "Precomp.h"
#include "IQMLoader.h"
#include "IQM.h"
#include "opengl/BufferObject.h"
#include "opengl/Mesh.h"
#include "utility/Logger.h"
#include "utility/Util.h"

IQMLoader::IQMLoader() {
  m_logger = GetContext().GetLogger();
}

IQMLoader::~IQMLoader() {
  m_logger = nullptr;
}

void IQMLoader::load_header(const char* data, iqmheader& header) {
  memcpy((void*)&header, (void*)data, sizeof(header));
}

bool IQMLoader::CheckByExtension(const std::string& ext) {
  return ext == "iqm" || ext == ".iqm";
}

MeshPtr IQMLoader::Load(const char* data, const uint32_t size) {
  MeshPtr glmesh;
  iqmheader head;

  load_header(data, head);

  if (!strcmp(IQM_MAGIC, head.magic) && head.version == IQM_VERSION && head.filesize > 0)
    m_logger->log(LOG_DEBUG, "IQM File appears to be correct and not empty. (MAGIC:%s,VERSION:%i)", head.magic, head.version);
  else {
    m_logger->log(LOG_DEBUG, "IQM file is corrupt or invalid.");
    return glmesh;
  }

  glmesh = MeshPtr(new Mesh());

  if (head.num_meshes > 0) {
    auto positions = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);
    auto texcoords = new BufferObject<glm::vec2>(IBufferObject::USAGE_HINT::STATIC);
    auto normals = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);
    auto tangents = new BufferObject<glm::vec4>(IBufferObject::USAGE_HINT::STATIC);
    auto bindexes = new BufferObject<glm::tvec4<uint8_t>>(IBufferObject::USAGE_HINT::STATIC);
    auto bweights = new BufferObject<glm::tvec4<uint8_t>>(IBufferObject::USAGE_HINT::STATIC);
    auto colors = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);
    auto indices = new IndexBufferObject<uint32_t>(IBufferObject::USAGE_HINT::STATIC);

    iqmmesh* submeshes;
    iqmvertexarray* vertexarrays;  // IQM vertex array info

    /// big single line of null terminated >strings<
    const char* texts = (const char*)&data[head.ofs_text];

    submeshes = (iqmmesh*)&data[head.ofs_meshes];
    vertexarrays = (iqmvertexarray*)&data[head.ofs_vertexarrays];

    indices->data.resize(head.num_triangles * 3);
    std::copy((uint32_t*)&data[head.ofs_triangles], (uint32_t*)&data[head.ofs_triangles + head.num_triangles * 3 * sizeof(uint32_t)],
              &indices->data[0]);

    for (uint32_t i = 0; i < head.num_vertexarrays; i++) {
      iqmvertexarray va = vertexarrays[i];

      switch (va.Type) {
        case IQM_POSITION:
          positions->data.resize(head.num_vertexes);
          std::copy((glm::vec3*)&data[va.offset], (glm::vec3*)&data[va.offset + head.num_vertexes * sizeof(glm::vec3)], &positions->data[0]);

          break;

        case IQM_TEXCOORD:
          texcoords->data.resize(head.num_vertexes);
          std::copy((glm::vec2*)&data[va.offset], (glm::vec2*)&data[va.offset + head.num_vertexes * sizeof(glm::vec2)], &texcoords->data[0]);

          break;

        case IQM_NORMAL:
          normals->data.resize(head.num_vertexes);
          std::copy((glm::vec3*)&data[va.offset], (glm::vec3*)&data[va.offset + head.num_vertexes * sizeof(glm::vec3)], &normals->data[0]);

          break;

        case IQM_TANGENT:
          tangents->data.resize(head.num_vertexes);
          std::copy((glm::vec4*)&data[va.offset], (glm::vec4*)&data[va.offset + head.num_vertexes * sizeof(glm::vec4)], &tangents->data[0]);

          break;

        case IQM_BLENDINDEXES:
          bindexes->data.resize(head.num_vertexes);
          std::copy((glm::tvec4<uint8_t>*)&data[va.offset], (glm::tvec4<uint8_t>*)&data[va.offset + head.num_vertexes * sizeof(glm::tvec4<uint8_t>)],
                    &bindexes->data[0]);

          break;

        case IQM_BLENDWEIGHTS:
          bweights->data.resize(head.num_vertexes);
          std::copy((glm::tvec4<uint8_t>*)&data[va.offset], (glm::tvec4<uint8_t>*)&data[va.offset + head.num_vertexes * sizeof(glm::tvec4<uint8_t>)],
                    &bweights->data[0]);

          break;

        case IQM_COLOR:
          colors->data.resize(head.num_vertexes);
          std::copy((glm::vec3*)&data[va.offset], (glm::vec3*)&data[va.offset + head.num_vertexes * sizeof(glm::vec3)], &colors->data[0]);

          break;
      }
    }

    for (int i = 0; i < indices->data.size(); i += 3) {
      auto ind = indices->data[i];
      indices->data[i] = indices->data[i + 2];
      indices->data[i + 2] = ind;
    }

    glmesh->buffers[IQM_POSITION] = positions;
    glmesh->buffers[IQM_TEXCOORD] = texcoords;
    glmesh->buffers[IQM_NORMAL] = normals;
    glmesh->buffers[IQM_TANGENT] = tangents;
    glmesh->buffers[IQM_BLENDINDEXES] = bindexes;
    glmesh->buffers[IQM_BLENDWEIGHTS] = bweights;
    glmesh->buffers[IQM_COLOR] = colors;
    glmesh->buffers[IQM_INDICES] = indices;

    glmesh->sub_meshes.resize(head.num_meshes);

    // mesh info test
    for (uint32_t i = 0; i < head.num_meshes; i++) {
      iqmmesh sm = submeshes[i];
      sub_mesh& m = glmesh->sub_meshes[i];
      m.name = &texts[sm.name];
      m.material_name = &texts[sm.material];
      m.start = sm.first_triangle * 3;
      m.num_indices = sm.num_triangles * 3;

      m_logger->log(LOG_DEBUG, "MESH LOADER INFO:\n\tName:%s\n\tMaterial:%s\n\tF.Vert:%i\n\tN.Verts:%i\n\tF.Ind:%i\n\tN.Inds:%i", m.name.c_str(),
                    m.material_name.c_str(), sm.first_vertex, sm.num_vertexes, sm.first_triangle * 3, sm.num_triangles * 3);
    }
  }

  loadiqmanims(glmesh, data, head);

  return glmesh;
}

void IQMLoader::loadiqmanims(std::shared_ptr<Mesh> m, const char* data, iqmheader& header) {
  if (header.num_poses != header.num_joints)
    return;

  const char* texts = (const char*)&data[header.ofs_text];
  iqmjoint* joints = (iqmjoint*)&data[header.ofs_joints];
  iqmpose* poses = (iqmpose*)&data[header.ofs_poses];
  iqmanim* anims = (iqmanim*)&data[header.ofs_anims];
  uint16_t* frame_data = (uint16_t*)&data[header.ofs_frames];

  m->animation_info.resize((int)header.num_anims);

  for (int i = 0; i < (int)header.num_anims; i++) {
    auto anim = std::make_shared<animation>();

    animation_info& ai = m->animation_info[i];
    iqmanim& a = anims[i];
    ai.name = &texts[a.name];
    ai.start = a.first_frame;
    ai.num = a.num_frames;
    ai.loop = CheckBit(a.flags, IQM_LOOP);

    m->animations[ai.name] = std::make_shared<animation>();
    auto& animation = m->animations[ai.name];
    animation->frames.resize(ai.num);

    for (uint32_t i = 0; i < ai.num; i++)
      animation->frames[i].resize(header.num_joints);

    animation->current_frame.resize(header.num_joints);
    animation->bones.resize(header.num_joints);

    auto base_frame = new glm::mat4[header.num_joints];
    auto inverse_base_frame = new glm::mat4[header.num_joints];

    for (uint32_t i = 0; i < header.num_joints; i++) {
      iqmjoint& j = joints[i];

      bone& b = animation->bones[i];
      b.name = &texts[j.name];
      b.parent = j.parent;
      b.pos = glm::vec3(j.translate[0], j.translate[1], j.translate[2]);
      b.scale = glm::vec3(j.scale[0], j.scale[1], j.scale[2]);
      b.rot = glm::normalize(glm::quat(j.rotate[3], j.rotate[0], j.rotate[1], j.rotate[2]));

      glm::mat4 jmat = glm::mat4(1.f);
      jmat = glm::translate(jmat, b.pos);
      jmat = glm::scale(jmat, b.scale);
      jmat = jmat * glm::toMat4(b.rot);

      base_frame[i] = jmat;
      inverse_base_frame[i] = glm::inverse(jmat);

      if (j.parent >= 0) {
        base_frame[i] = base_frame[j.parent] * base_frame[i];
        inverse_base_frame[i] = inverse_base_frame[i] * inverse_base_frame[j.parent];
      }
    }

    auto& bones = animation->bones;
    for (int i = 0; i < bones.size(); i++) {
      if (bones[i].parent < 0)
        continue;  // this is a root bone

      auto& bone = bones[i];
      auto& parent = bones[bone.parent];
      parent.children.push_back(&bone);
    }

    for (int i = 0; i < bones.size(); i++) {
      {
        auto bone = bones[i];
        auto& children = bones[i].children;
        for (int j = 0; j < children.size(); j++) {
          auto child = children[j];
          child->pos = bone.pos + bone.rot * (bone.scale * child->pos);
          child->rot = bone.rot * child->rot;
        }
        children.clear();
      }
    }

    for (int i = 0; i < (int)ai.num; i++) {
      for (int j = 0; j < (int)header.num_poses; j++) {
        iqmpose& p = poses[j];
        glm::quat rotate;
        glm::vec3 translate, scale;
        translate.x = p.channeloffset[0];
        if (p.mask & 0x01)
          translate.x += *frame_data++ * p.channelscale[0];
        translate.y = p.channeloffset[1];
        if (p.mask & 0x02)
          translate.y += *frame_data++ * p.channelscale[1];
        translate.z = p.channeloffset[2];
        if (p.mask & 0x04)
          translate.z += *frame_data++ * p.channelscale[2];
        rotate.x = p.channeloffset[3];
        if (p.mask & 0x08)
          rotate.x += *frame_data++ * p.channelscale[3];
        rotate.y = p.channeloffset[4];
        if (p.mask & 0x10)
          rotate.y += *frame_data++ * p.channelscale[4];
        rotate.z = p.channeloffset[5];
        if (p.mask & 0x20)
          rotate.z += *frame_data++ * p.channelscale[5];
        rotate.w = p.channeloffset[6];
        if (p.mask & 0x40)
          rotate.w += *frame_data++ * p.channelscale[6];
        scale.x = p.channeloffset[7];
        if (p.mask & 0x80)
          scale.x += *frame_data++ * p.channelscale[7];
        scale.y = p.channeloffset[8];
        if (p.mask & 0x100)
          scale.y += *frame_data++ * p.channelscale[8];
        scale.z = p.channeloffset[9];
        if (p.mask & 0x200)
          scale.z += *frame_data++ * p.channelscale[9];

        /// Concatenate each pose with the inverse base pose to avoid doing this at animation time.
        /// If the joint has a parent, then it needs to be pre-concatenated with its parent's base pose.
        /// Thus it all negates at animation time like so:
        ///   (parentPose * parentInverseBasePose) * (parentBasePose * childPose * childInverseBasePose) =>
        ///   parentPose * (parentInverseBasePose * parentBasePose) * childPose * childInverseBasePose =>
        ///   parentPose * childPose * childInverseBasePose

        glm::mat4 mat = glm::mat4(1.f);
        mat = glm::translate(mat, translate);
        mat = glm::scale(mat, scale);
        mat = mat * glm::toMat4(glm::normalize(rotate));

        if (p.parent >= 0) {
          animation->frames[i][j] = base_frame[p.parent] * mat * inverse_base_frame[j];
        } else {
          animation->frames[i][j] = mat * inverse_base_frame[j];
        }
      }
    }

    delete[] base_frame;
    delete[] inverse_base_frame;

    m_logger->log(LOG_DEBUG, "Loaded anim: %s. start: %d frames: %d framerate: %d loop: %s", ai.name.c_str(), ai.start, ai.num, ai.framerate,
                  ai.loop ? "true" : "false");
  }
}