#include "lab_m1/race/obstacle.h"

using namespace std;
using namespace m1;


Obstacle::~Obstacle()
{
}

glm::vec3 m1::Obstacle::GetPositionAndAdvance()
{
	glm::vec3 position = route[idx];

	int size = (int)route.size();
	idx += 10;
	if (idx == size) {
		idx = 0;
	}

	return position;
}

bool m1::Obstacle::HasCollidedWith(glm::vec3 player_position)
{
	return false;
}
