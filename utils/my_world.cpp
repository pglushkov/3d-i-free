
#include "my_world.h"

MyProjectionMatrix<float> MyWorld::world_proj(WORLD_DEFAULT_NEAR_PLANE,
					       WORLD_DEFAULT_FAR_PLANE,
					       WORLD_DEFAULT_ASPECT_RATIO,
					       WORLD_DEFAULT_FOV_ANGLE);

MyPositionMatrix<float> MyWorld::camera_view;
MySquareMatrix<float, 4> MyWorld::result_camera_view;

float MyWorld::creation_time = my_utils::GetTimeInSec();
std::vector<MyWorld::light_pos_vector> MyWorld::global_lights(1);

MyWorld::MyWorld()
{
	/* When World is created - at least on global light source is created! */
	global_lights[0] = light_pos_vector( {0.0f, 0.0f, 1.0f} );
        UpdateCameraView();
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


void MyWorld::RotateCamera_X1(float angle_deg)
{
        camera_view.Rotate_X(angle_deg);
        UpdateCameraView();
        camera_view.TRACE_AXIS_GLOB();
}

void MyWorld::RotateCamera_Y1(float angle_deg)
{
        MyPositionMatrix<float>::data_row y_axis({0.0f, 1.0f, 0.0f, 1.0f});
        MySquareMatrix<float, 4> inv = camera_view.get_data().Invert();
        MyPositionMatrix<float>::data_row new_y = inv * y_axis;
        new_y[2] = - new_y[2];
        new_y[0] -= inv[0][3];
        new_y[1] -= inv[1][3];
        new_y[2] -= -inv[2][3];
        camera_view.Rotate_Axis(new_y, angle_deg);
//        camera_view.RotateGlobal_Y(angle_deg);
        UpdateCameraView();
        camera_view.TRACE_AXIS_GLOB();
}
