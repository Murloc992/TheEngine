#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "opengl/Mesh.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"

class SphereMesh
{
private:
	float m_radius;
public:
	BufferObject<glm::vec3> * pos;
	BufferObject<glm::tvec4<uint8_t>> * col;
	BufferObject<glm::vec2> * tex_coords;
	BufferObject<glm::vec3> * normals;
	IndexBufferObject<uint32_t> * indices;
	glm::tvec4<uint8_t> m_color;

	std::shared_ptr<Mesh> glmesh;

	SphereMesh(float radius = 1.0f, glm::tvec4<uint8_t> color = glm::tvec4<uint8_t>(1))
	{
		this->m_radius = radius;
		this->m_color = color;
		Init();
	}

	~SphereMesh() {};

	bool Init(const int32_t& stack_count = 18, const int32_t& sector_count = 36)
	{
		glmesh = share(new Mesh());

		pos = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);

		normals = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);

		tex_coords = new BufferObject<glm::vec2>(IBufferObject::USAGE_HINT::STATIC);

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / m_radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		const auto pi = glm::pi<float>();
		float sectorStep = 2 * pi / sector_count;
		float stackStep = pi / stack_count;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stack_count; ++i)
		{
			stackAngle = pi / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = m_radius * cosf(stackAngle);             // r * cos(u)
			z = m_radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sector_count; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

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

		col = new BufferObject<glm::tvec4<uint8_t>>(IBufferObject::USAGE_HINT::STATIC);
		col->data.resize(pos->data.size(), m_color);

		indices = new IndexBufferObject<uint32_t>(IBufferObject::USAGE_HINT::STATIC);

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
					indices->data.push_back(k1+1);
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

		glmesh->buffers[Mesh::POSITION] = pos;
		glmesh->buffers[Mesh::COLOR] = col;
		glmesh->buffers[Mesh::INDICES] = indices;
		glmesh->buffers[Mesh::NORMAL] = normals;
		glmesh->buffers[Mesh::TEXCOORD] = tex_coords;

		glmesh->Init();
		return true;
	}

	void Render(bool wireframe, bool ccw_winding = true)
	{
		if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (!ccw_winding) glFrontFace(GL_CW);
		glmesh->Render();
		if (!ccw_winding) glFrontFace(GL_CCW);
		if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
};

#endif // SPHEREMESH_H
