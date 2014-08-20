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
        static MySquareMatrix<float, 4>& GetWorldCameraView() { return result_camera_view; }

        static void RotateCamera_X(float angle_deg) { camera_view.Rotate_X(angle_deg); UpdateCameraView(); }
        static void RotateCamera_Y(float angle_deg) { camera_view.Rotate_Y(angle_deg); UpdateCameraView(); }
        static void RotateCamera_Z(float angle_deg) { camera_view.Rotate_Z(angle_deg); UpdateCameraView(); }
        static void RotateCamera_X1(float angle_deg);
        static void RotateCamera_Y1(float angle_deg);
        static void RotateCamera_Axis(float x, float y, float z, float angle_deg)
        {
                camera_view.Rotate_Axis(x, y, z, angle_deg);
                UpdateCameraView();
        }

        static void MoveCamera_Z(float step)
        {
                camera_view.Translate( MyPositionMatrix<float>::translation_vector({0.0f, 0.0f, step}) );
                UpdateCameraView();
        }
        static void MoveCamera_X(float step)
        {
                camera_view.Translate( MyPositionMatrix<float>::translation_vector({step, 0.0f, 0.0f}) );
                UpdateCameraView();
        }
        static void MoveCamera_Y(float step)
        {
                camera_view.Translate( MyPositionMatrix<float>::translation_vector({0.0f, step, 0.0f}) );
                UpdateCameraView();
        }
        static void ResetCameraPos() { camera_view.Reset(); UpdateCameraView(); }
        static void CameraStepForward(float step);
        static void CameraStepSide(float step);

        static const char* Default_VShader() { return "../shaders/vshader_full.txt"; }
        static const char* Default_FShader() { return "../shaders/fshader_simple.txt"; }

private:

	static MyProjectionMatrix<float> world_proj;
	static float creation_time;
	static std::vector<light_pos_vector> global_lights;
	static MyPositionMatrix<float> camera_view;
	static MySquareMatrix<float, 4> result_camera_view;
	static MySquareMatrix<float, 4>::data_row camera_direction;

	MyWorld& operator=(MyWorld& in) { return *this; }
	MyWorld(MyWorld& in) { }
	MyWorld();
	static void UpdateCameraView();
};

#endif // MY_WORLD_H
