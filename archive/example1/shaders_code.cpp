
#include "shaders_code.h"


const std::string shaders_code::simple_v_shader =
"\
varying vec2 texCoord; \
void main()\
{\
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
        texCoord = vec2(gl_Position.x, gl_Position.y);\
}\
"
;

const std::string shaders_code::simple_f_shader =
"\
varying vec2 texCoord;\
void main()\
{\
        /*if ( gl_FragCoord.y > 300.0 && gl_FragCoord.x > 500.0) {\
                gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\
        } else {\
                gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\
        }*/\
\
        float R = sqrt(texCoord.x * texCoord.x + texCoord.y*texCoord.y);\
        if ( R < 0.5 && R > 0.3)\
                gl_FragColor = vec4(R*2.0, R*3.0, 0.5, 1.0);\
        else\
                gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\
}\
"
;

