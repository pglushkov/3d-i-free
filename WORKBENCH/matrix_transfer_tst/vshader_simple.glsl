#version 130

attribute int in_mat_idx;
attribute vec4 in_vertex_position;

uniform float in_intensity;
uniform mat4 tst_matrix;


varying float intensity;
varying vec4 var_color;

void main()
{
        gl_Position =in_vertex_position;

        var_color = tst_matrix[in_mat_idx];
        intensity = in_intensity;
}

