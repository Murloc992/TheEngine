#include "opengl/OpenGLInc.h"

#include "SphereMesh.h"

SphereMesh::SphereMesh(float radius, const int32_t stack_count, const int32_t sector_count, bool flipped_normals, glm::tvec4<uint8_t> color)
{
	m_radius = radius;
	m_color = color;

	BufferObject<glm::vec3>* pos = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);

	BufferObject<glm::vec3>* normals = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);

	BufferObject<glm::vec2>* tex_coords = new BufferObject<glm::vec2>(IBufferObject::USAGE_HINT::STATIC);

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz;
	float lengthInv = 1.0f / m_radius;    // vertex normal
	if (flipped_normals) lengthInv = -1.0f / m_radius;
	float s, t;                                     // vertex texCoord

	const auto pi = glm::pi<float>();
	float sectorStep = (2.f * pi) / sector_count;
	float stackStep = pi / stack_count;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stack_count; ++i)
	{
		stackAngle = pi / (2.f - i * stackStep);        // starting from pi/2 to -pi/2
		xy = m_radius * glm::cos(stackAngle);             // r * cos(u)
		z = m_radius * glm::sin(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sector_count; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * glm::cos(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * glm::sin(sectorAngle);             // r * cos(u) * sin(v)

			pos->data.push_back(glm::vec3(x, y, z));

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			normals->data.push_back(glm::vec3(nx, ny, nz));

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sector_count;
			t = (float)i / stack_count;

			tex_coords->data.push_back(glm::vec2(s, t));
		}
	}

	BufferObject<glm::tvec4<uint8_t>>* col = new BufferObject<glm::tvec4<uint8_t>>(IBufferObject::USAGE_HINT::STATIC);
	col->data.resize(pos->data.size(), m_color);

	IndexBufferObject<uint32_t>* indices = new IndexBufferObject<uint32_t>(IBufferObject::USAGE_HINT::STATIC);

	// generate CCW index list of sphere triangles
	int k1, k2;
	for (int i = 0; i < stack_count; ++i)
	{
		k1 = i * (sector_count + 1);     // beginning of current stack
		k2 = k1 + sector_count + 1;      // beginning of next stack

		for (int j = 0; j < sector_count; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices->data.push_back(k1);
				indices->data.push_back(k2);
				indices->data.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stack_count - 1))
			{
				indices->data.push_back(k1 + 1);
				indices->data.push_back(k2);
				indices->data.push_back(k2 + 1);
			}
		}
	}

	buffers[Mesh::POSITION] = pos;
	buffers[Mesh::COLOR] = col;
	buffers[Mesh::INDICES] = indices;
	buffers[Mesh::NORMAL] = normals;
	buffers[Mesh::TEXCOORD] = tex_coords;

	Init();
}