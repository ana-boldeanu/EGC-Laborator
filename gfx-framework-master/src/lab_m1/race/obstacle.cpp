#include "lab_m1/race/obstacle.h"

using namespace std;
using namespace m1;

#include <iostream>

Obstacle::~Obstacle()
{
}

glm::vec3 Obstacle::GetPosition()
{
	return route[idx];
}

glm::vec3 Obstacle::GetPositionAndAdvance()
{
	glm::vec3 position = route[idx];

	int size = (int)route.size();
	idx -= 1;
	if (idx < 0) {
		idx = size - 1;
	}

	return position;
}


bool Obstacle::SpheresCollision(glm::vec3 center_A, glm::vec3 center_B, float radius_A, float radius_B)
{
	float distance = glm::distance(center_A, center_B);
	float radii_sum = radius_A + radius_B;

	if (distance <= radii_sum) {
		return true;
	}
	
	return false;
}


bool Obstacle::HasCollidedWith(glm::vec3 player_pos)
{
	glm::vec3 position = GetPosition();
	position *= road_scale;
	
	return SpheresCollision(position, player_pos, radius, car_radius);
}
