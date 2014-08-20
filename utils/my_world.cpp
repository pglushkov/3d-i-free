
#include "my_world.h"

MyProjectionMatrix<float> MyWorld::world_proj(WORLD_DEFAULT_NEAR_PLANE,
					       WORLD_DEFAULT_FAR_PLANE,
					       WORLD_DEFAULT_ASPECT_RATIO,
					       WORLD_DEFAULT_FOV_ANGLE);

MyPositionMatrix<float> MyWorld::camera_view;
MySquareMatrix<float, 4> MyWorld::result_camera_view;
MySquareMatrix<float, 4>::data_row MyWorld::camera_direction;
MySquareMatrix<float, 4>::data_row MyWorld::camera_side_direction;

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
//        camera_view.TRACE_AXIS_GLOB();
}

void MyWorld::RotateCamera_Y1(float angle_deg)
{
        // In order to provide more convinient camera control, we should freeze the Y-axis of a camera transform.
        // In other words - when we rotate camera around Y-axis (i.e. left-to-right) we always rotate camera around
        // axis that is parallel to global Y-axis in world coordinates and starts at position of a camera.

        // recover global Y-axis in world coordinates from transform of a camera
        MyPositionMatrix<float>::data_row y_axis({0.0f, 1.0f, 0.0f, 1.0f});
        MySquareMatrix<float, 4> inv = camera_view.get_data().Invert();
        MyPositionMatrix<float>::data_row new_y = inv * y_axis;
        new_y[2] = - new_y[2];

        // putting global Y-axis origin at actual position of a camera
        new_y[0] -= inv[0][3];
        new_y[1] -= inv[1][3];
        new_y[2] -= -inv[2][3];

        // performing actual rotation
        camera_view.Rotate_Axis(new_y, angle_deg);

        UpdateCameraView();
//        camera_view.TRACE_AXIS_GLOB();

//        std::cout << "glob_y.x=" << new_y[0] << " glob_y.y="
//                  << new_y[1] << " glob_y.z=" << new_y[2] << std::endl;
}

void MyWorld::CameraStepForward(float step)
{
       float x_inc = camera_direction[0] * step;
       float y_inc = camera_direction[1] * step;
       float z_inc = camera_direction[2] * step;

       MoveCamera_X(-x_inc);
       MoveCamera_Y(y_inc);
       MoveCamera_Z(-z_inc);

//       UpdateCameraView();
}

void MyWorld::CameraStepSide(float step)
{

       float x_inc = camera_side_direction[0] * step;
       float y_inc = camera_side_direction[1] * step;
       float z_inc = camera_side_direction[2] * step;

       MoveCamera_X(-x_inc);
       MoveCamera_Y(y_inc);
       MoveCamera_Z(-z_inc);
}


void MyWorld::UpdateCameraView()
{
        MySquareMatrix<float, 4> inv = camera_view.get_data().Invert();
        result_camera_view = inv;
        MySquareMatrix<float, 4>::data_row axis({0.0f, 0.0f, -1.0f, 1.0f});
        MySquareMatrix<float, 4>::data_row x_axis({1.0f, 0.0f, 0.0f, 1.0f});
        camera_direction = camera_view.get_data() * axis;
        camera_direction[1] = -camera_direction[1];
        camera_direction[2] = camera_direction[2];

        camera_direction[0] -= camera_view.get_data()[0][3];
        camera_direction[1] -= -camera_view.get_data()[1][3];
        camera_direction[2] -= camera_view.get_data()[2][3];

        camera_side_direction = camera_view.get_data() * x_axis;
        camera_side_direction[0] -= camera_view.get_data()[0][3];
        camera_side_direction[1] -= camera_view.get_data()[1][3];
        camera_side_direction[2] -= camera_view.get_data()[2][3];

        // DBG
        std::cout << "direction.x=" << camera_direction[0] << " direction.y="
                  << camera_direction[1] << " direction.z=" << camera_direction[2] << std::endl;
        std::cout << "glob_pos.x=" << inv[0][3] << " glob_pos.y="
                  << inv[1][3] << " glob_pos.z=" << inv[2][3] << std::endl;
        std::cout << "s_direction.x=" << camera_side_direction[0] << " s_direction.y="
                  << camera_side_direction[1] << " s_direction.z=" << camera_side_direction[2] << std::endl;
}

