#version 130

varying float intensity;
varying vec4 var_color;

void main()
{

//        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0f) * intensity;
    gl_FragColor = var_color * intensity;
}

