#shader vertex
#version 400

layout(location = 0) in vec3 position;

uniform mat4 u_MVP;


void main()
{
    gl_Position = u_MVP * vec4(position, 1.0);
};

#shader fragment
#version 400


layout(location = 0) out vec4 frag_Color;

uniform vec4 u_Color;


void main()
{
    
    frag_Color = u_Color;

};