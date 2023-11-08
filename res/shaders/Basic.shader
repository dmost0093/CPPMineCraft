#version 400

layout(location = 0) in vec3 position;

//uniform mat4 u_Model;
//uniform mat4 u_ViewProjection;

void main()
{
    gl_Position =  vec4(position, 1.0);
};

#shader fragment
#version 400

uniform vec4 u_Color;

layout(location = 0) out vec4 frag_Color;

void main()
{
    
    frag_Color = u_Color;

};