#ifndef MY_WORLD_H
#define MY_WORLD_H

#include "misc_utils.h"
#include "opengl.h"
#include "simple_vectors.h"
#include "my_position_matrix.h"


#define WORLD_DEFAULT_FAR_PLANE 100.0f
#define WORLD_DEFAULT_NEAR_PLANE 0.5f
#define WORLD_DEFAULT_ASPECT_RATIO 1.3333333333f
#define WORLD_DEFAULT_FOV_ANGLE 35.0f

class MyWorld
{
public:
	typedef std::array<float, 3> light_pos_vector;

	static const MyWorld& GetInstance()
	{
		static MyWorld inst;

		return inst;
	}

	static MyProjectionMatrix<float>& GetWorldProjection() { return world_proj; }
	static float GetCreationTime() { return creation_time; }
	static float GetTimeSinceCreation();
	const light_pos_vector& GetGlobalLightPos( size_t idx = 0 ) const;

private:

	static MyProjectionMatrix<float> world_proj;
	static float creation_time;
	static std::vector<light_pos_vector> global_lights;

	MyWorld& operator=(MyWorld& in) { return *this; }
	MyWorld(MyWorld& in) { }
	MyWorld();
};

#endif // MY_WORLD_H
