
#include "my_world.h"

MyProjectionMatrix<float> MyWorld::world_proj(WORLD_DEFAULT_NEAR_PLANE,
					       WORLD_DEFAULT_FAR_PLANE,
					       WORLD_DEFAULT_ASPECT_RATIO,
					       WORLD_DEFAULT_FOV_ANGLE);

float MyWorld::creation_time = my_utils::GetTimeInSec();
std::vector<MyWorld::light_pos_vector> MyWorld::global_lights(1);

MyWorld::MyWorld()
{
	/* When World is created - at least on global light source is created! */
	global_lights[0] = light_pos_vector( {0.0f, 0.0f, 1.0f} );
}

float MyWorld::GetTimeSinceCreation()
{
	return my_utils::GetTimeInSec() - creation_time;
}

const MyWorld::light_pos_vector& MyWorld::GetGlobalLightPos( size_t idx ) const
{
	if (idx >= global_lights.size())
		throw std::runtime_error("ERROR! Trying to access non-existant global lights source! Index out of bounds!");

	return global_lights[idx];
}
