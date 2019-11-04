#include "Precomp.h"
#include "Util.h"
#include <iomanip>

void print_glm(const glm::mat4 & m)
{
	std::cout << std::setw(2) << std::setprecision(3) << "[" << m[0].x << "," << m[0].y << "," << m[0].z << "," << m[0].w << "]\n";
	std::cout << std::setw(2) << std::setprecision(3) << "[" << m[1].x << "," << m[1].y << "," << m[1].z << "," << m[1].w << "]\n";
	std::cout << std::setw(2) << std::setprecision(3) << "[" << m[2].x << "," << m[2].y << "," << m[2].z << "," << m[2].w << "]\n";
	std::cout << std::setw(2) << std::setprecision(3) << "[" << m[3].x << "," << m[3].y << "," << m[3].z << "," << m[3].w << "]\n\n";
}

void print_glm2(const glm::mat4& m)
{
	std::cout << std::setw(2) << std::setprecision(3) << "[" << m[0].x << "," << m[1].x << "," << m[2].x << "," << m[3].x << "]\n";
	std::cout << std::setw(2) << std::setprecision(3) << "[" << m[0].y << "," << m[1].y << "," << m[2].y << "," << m[3].y << "]\n";
	std::cout << std::setw(2) << std::setprecision(3) << "[" << m[0].z << "," << m[1].z << "," << m[2].z << "," << m[3].z << "]\n";
	std::cout << std::setw(2) << std::setprecision(3) << "[" << m[0].w << "," << m[1].w << "," << m[2].w << "," << m[3].w << "]\n\n";
}