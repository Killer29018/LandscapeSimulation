#version 430

in vec2 v_Texcoords;
in flat vec3 v_Normal;

out vec4 f_Colour;

void main()
{
    f_Colour = vec4(v_Texcoords, 0.0, 1.0);
}
