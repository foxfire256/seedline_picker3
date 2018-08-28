#version 330
smooth in vec3 intensity;

out vec4 out_color;

void main()
{
	out_color = vec4(intensity, 1.0);
}
