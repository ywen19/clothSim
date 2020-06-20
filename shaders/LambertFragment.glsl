#version 410 core


in vec3 lightIntensity;
layout (location =0) out vec4 fragColour;

void main ()
{
  fragColour=vec4(0.85f, 0.6f, 0.0f, 1.0f) * vec4(lightIntensity,1.f);
}
