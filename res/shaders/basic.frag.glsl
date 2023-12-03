#version 430

in vec3 v_Colour;

out vec4 f_Colour;

void main()
{
    f_Colour = vec4(v_Colour, 1.0);
}
