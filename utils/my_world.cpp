
#include "my_world.h"

MyProjectionMatrix<float> MyWorld::world_proj(WORLD_DEFAULT_NEAR_PLANE,
					       WORLD_DEFAULT_FAR_PLANE,
					       WORLD_DEFAULT_ASPECT_RATIO,
					       WORLD_DEFAULT_FOV_ANGLE);

MyPositionMatrix<float> MyWorld::camera_view;
MySquareMatrix<float, 4> MyWorld::result_camera_view;
MySquareMatrix<float, 4>::data_row MyWorld::camera_direction;
MySquareMatrix<float, 4>::data_row MyWorld::camera_right_direction;
MySquareMatrix<float, 4>::data_row MyWorld::camera_position;
MyMaterial MyWorld::default_mater;

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
        // In order to provide more convinient camera control, we should 'freeze' the Y-axis of a camera transform.
        // In other words - when we rotate camera around Y-axis (i.e. left-to-right) we always rotate camera around
        // global Y axis of camera space. This axis is also parallel to world's Y axis but starts at camera's position

        // recover global Y-axis in camera coordinates from transform of a camera
        MyPositionMatrix<float>::data_row y_axis({0.0f, 1.0f, 0.0f, 1.0f});
        MyPositionMatrix<float>::data_row new_y = y_axis * camera_view.get_data();

        // performing actual rotation
        camera_view.Rotate_Axis(new_y, angle_deg);

        UpdateCameraView();

//        std::cout << "glob_y.x=" << new_y[0] << " glob_y.y="
//                  << new_y[1] << " glob_y.z=" << new_y[2] << std::endl;

}

void MyWorld::CameraStepForward(float step)
{
       float x_inc = camera_direction[0] * step;
       float y_inc = camera_direction[1] * step;
       float z_inc = camera_direction[2] * step;

       // default camera orientation is so, that it's
       // 1) camera's z axis is alligned with worlds' -z axis
       // 2) camera's x axis is alligned with worlds' -x axis
       // 3) camera's y axis is alligned with worlds' y axis
       // camera_right_direction show's camera's -X-direction in WORLD coordinates, so have to invert x and z to see it's direction in CAMERA coordinates

       camera_view.Translate( MyPositionMatrix<float>::translation_vector({x_inc, -y_inc, z_inc}) );
       UpdateCameraView();
}

void MyWorld::CameraStepSide(float step)
{

       float x_inc = camera_right_direction[0] * step;
       float y_inc = camera_right_direction[1] * step;
       float z_inc = camera_right_direction[2] * step;

       // default camera orientation is so, that it's
       // 1) camera's z axis is alligned with worlds' -z axis
       // 2) camera's x axis is alligned with worlds' -x axis
       // 3) camera's y axis is alligned with worlds' y axis
       // camera_right_direction show's camera's -X-direction in WORLD coordinates, so have to invert x and z to see it's direction in CAMERA coordinates

       camera_view.Translate( MyPositionMatrix<float>::translation_vector({x_inc, -y_inc, z_inc}) );
       UpdateCameraView();
}


void MyWorld::UpdateCameraView()
{
        // First, we recover camera's direction in global world coordinates.
        // To do so, we have to take initial position of camera direction in worlds coordinates, which is (0,0,-1)
        // and multiply it by inverse of camera's transform - same way we do it with vertices in vertex shader.
        // Same thing with right-direction (which is right if looking out of camera and (1,0,0) in world coordinates).
        MySquareMatrix<float, 4> inv = camera_view.get_data().Invert();
        result_camera_view = inv;
        MySquareMatrix<float, 4>::data_row z_axis({0.0f, 0.0f, 1.0f, 1.0f});
        MySquareMatrix<float, 4>::data_row x_axis({-1.0f, 0.0f, 0.0f, 1.0f});
        camera_direction = z_axis * camera_view.get_data();
        camera_right_direction = x_axis * camera_view.get_data() ;

        // camera space is oriented inversly to world in X and Z ...
        camera_position[0] = -camera_view.get_data()[0][3];
        camera_position[1] = camera_view.get_data()[1][3];
        camera_position[2] = -camera_view.get_data()[2][3];

        // working in such a mode, that first world's global light is positioned 'inside' the camera
        global_lights[0][0] = camera_position[0];
        global_lights[0][1] = camera_position[1];
        global_lights[0][2] = camera_position[2];

        // DBG
        std::cout << "direction.x=" << camera_direction[0] << " direction.y="
                  << camera_direction[1] << " direction.z=" << camera_direction[2] << std::endl;

//        std::cout << "camera_pos.x=" << camera_position[0] << " camera_pos.y="
//                  << camera_position[1] << " camera_pos.z=" << camera_position[2] << std::endl;

//        std::cout << "s_direction.x=" << camera_right_direction[0] << " s_direction.y="
//                  << camera_right_direction[1] << " s_direction.z=" << camera_right_direction[2] << std::endl;

//        std::cout << "light_position.x=" << global_lights[0][0] << " light_position.y="
//                  << global_lights[0][1] << " light_position.z=" << global_lights[0][2] << std::endl;
}

