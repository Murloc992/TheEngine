#ifndef PLANE3D_H_INCLUDED
#define PLANE3D_H_INCLUDED

class Plane3d
{
public:
	///  Points are going to be passed in counter-clockwise
	///  pC--------pD
	///  |       /  |
	///  |     /    |
	///  |   /      |
	///  | /        |
	///  pB--------pA

	Plane3d()
	{
		n = glm::normalize(glm::vec4(1));
	}

	~Plane3d()
	{
	}

	void SetNormalsAndD(const glm::vec4 &normal)
	{
		this->n = normal;

		Normalize();
	}

	void SetPoints(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4)
	{
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
		points[3] = p4;
	}

	void Normalize()
	{
		n = glm::normalize(n);
		glm::vec3 normal(n.x, n.y, n.z);
		float length = glm::length(normal);
		n = n / length;
	}

	const glm::vec3* GetPoints() const
	{
		return points;
	}

	float Distance(const glm::vec3 &p, float radius = 0.f)
	{
		auto distance = (n.w + radius) + glm::dot(glm::vec3(n), p);
		return distance;
	}
private:
	glm::vec3 points[4];
	glm::vec4 n;
protected:
};

#endif
