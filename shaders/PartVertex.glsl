#version 330 core

layout (location=0) in vec3 inVert;
layout (location=1) in vec3 inColour;

uniform mat4 MVP;
out vec3 particleColour;

void main()
{
    particleColour = inColour;
    gl_Position=MVP*vec4(inVert,1.0);
}
