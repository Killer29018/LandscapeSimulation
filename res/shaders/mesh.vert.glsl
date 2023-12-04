#version 430

layout (location = 0) in vec3 i_Position;
layout (location = 1) in vec2 i_Texcoords;

struct HeightData
{
    vec4 height;
};

layout (std430, binding = 1) readonly buffer HeightDataBuffer
{
    HeightData data[];
} s_HeightData;

out vec2 v_Texcoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform int u_SideLength;

void main()
{
    int index = gl_VertexID;

    vec3 offset = vec3(0.);
    offset.y = s_HeightData.data[index].height.y;
    vec3 newPosition = i_Position + offset;

    gl_Position = u_Projection * u_View * u_Model * vec4(newPosition, 1.0);

    v_Texcoords = i_Texcoords;
}
