#version 430

layout (location = 0) in vec3 i_Position;
layout (location = 1) in vec2 i_Texcoords;

uniform sampler2D u_HeightTexture;

out vec2 v_Texcoords;
out vec3 v_Normal;
out float v_Height;

out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec2 u_HeightTextureSize;

const float c_Multiplier = 50;
const float c_Offset = 0.0;

float getRawHeight(vec2 position)
{
    return (texture(u_HeightTexture, position).r + c_Offset);
}

float getHeight(vec2 position)
{
    return getRawHeight(position) * c_Multiplier;
}

void main()
{
    int index = gl_VertexID;

    vec3 offset = vec3(0.);
    offset.y = getHeight(i_Texcoords);
    vec3 newPosition = i_Position + offset;

    gl_Position = u_Projection * u_View * u_Model * vec4(newPosition, 1.0);

    vec2 increment = vec2(1.0) / u_HeightTextureSize;
    vec3 increment3 = vec3(increment.x, 0., increment.y);


    vec2 dTexcoords = i_Texcoords + increment3.yz;
    float d = getHeight(dTexcoords);

    vec2 uTexcoords = i_Texcoords - increment3.yz;
    float u = getHeight(uTexcoords);

    vec2 lTexcoords = i_Texcoords - increment3.xz;
    float l = getHeight(lTexcoords);

    vec2 rTexcoords = i_Texcoords + increment3.xz;
    float r = getHeight(rTexcoords);

    vec3 ud = vec3(0, u - d, increment3.z * 2);
    vec3 lr = vec3(increment3.x * 2, l - r, 0);
    vec3 normal = normalize(cross(ud, lr));

    v_Texcoords = i_Texcoords;

    v_FragPos = vec3(u_Model * vec4(newPosition, 1.0));
    v_Normal = normal;
    v_Height = getRawHeight(i_Texcoords);
}
