#version 130

attribute int in_mat_idx;
attribute vec4 in_vertex_position;

uniform float in_angle_x;
uniform float in_angle_y;
uniform float in_angle_z;
uniform float in_x_offset;
uniform float in_y_offset;
uniform float in_z_offset;
uniform mat4 tst_matrix;


varying vec4 var_color;
varying float dist;

void main()
{
        mat4 proj = mat4(

                        vec4(1.0, 0.0, 0.0, 0.0),
                        vec4(0.0, 1.0, 0.0, 0.0),
                        vec4(0.0, 0.0, 1.0, 0.0),
                        vec4(0.0, 0.0, 0.0, 1.0)

                    );

        float pi = 3.14159265;


        float angle_rad = in_angle_x / 180.0 * pi;
        float C = cos(angle_rad);
        float S = sin(angle_rad);
        mat4 x_rot = mat4(

                    vec4(1.0, 0.0, 0.0, 0.0), // first column, not row!!!
                    vec4(0.0, C, S, 0.0),  // second column !!
                    vec4(0.0, -S, C, 0.0), // third column
                    vec4(0.0, 0.0, 0.0, 1.0) // fourth column

                    );


        angle_rad = in_angle_y / 180.0 * pi;
        C = cos(angle_rad);
        S = sin(angle_rad);
        mat4 y_rot = mat4(

                    vec4(C, 0.0, -S, 0.0), // first column, not row!!!
                    vec4(0.0, 1.0, 0.0, 0.0),  // second column !!
                    vec4(S, 0.0, C, 0.0), // third column
                    vec4(0.0, 0.0, 0.0, 1.0) // fourth column

                    );

        angle_rad = in_angle_z / 180.0 * pi;
        C = cos(angle_rad);
        S = sin(angle_rad);
        mat4 z_rot = mat4(

                    vec4(C, S, 0.0, 0.0), // first column, not row!!!
                    vec4(-S, C, 0.0, 0.0),  // second column !!
                    vec4(0.0, 0.0, 1.0, 0.0), // third column
                    vec4(0.0, 0.0, 0.0, 1.0) // fourth column

                    );

        mat4 model_transform = x_rot * y_rot * z_rot;
        vec4 pos_orto = model_transform * (  in_vertex_position + vec4(in_x_offset, in_y_offset, in_z_offset, 0.0)  );

        var_color = tst_matrix[in_mat_idx];
        dist = (pos_orto[2] + 0.5 );

        gl_Position = proj * pos_orto;
}

