#version 130

varying vec4 var_color;
varying float dist;

void main()
{

//        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0f) * intensity;
    gl_FragColor = var_color * dist;
}

