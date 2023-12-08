#version 430

in vec2 v_Texcoords;
in vec3 v_Normal;
in vec3 v_FragPos;
in float v_Height;

uniform vec3 u_CameraView;
uniform vec3 u_CameraPosition;

uniform sampler2D u_HeightTexture;

out vec4 f_Colour;

const vec3 c_LightPosition = vec3(0, 100, 0);
const float c_Diffuse = 1.0;
const vec3 c_SurfaceColour = vec3(0.7, 0.4, 0.5);
const vec3 c_LightColour = vec3(0.5);
const float c_Ambient = 0.4;
const vec3 c_AmbientColour = vec3(1.0);
const float c_Specular = 0.01;
const int c_Alpha = 50;

const float c_Offset = 0.3;

void main()
{
    vec3 colour = vec3(0.);
    if (v_Height > 0.8) colour = vec3(0.9, 0.5, 0.5);
    else if (v_Height > 0.5) colour = vec3(0.8, 0.4, 0.4);
    else if (v_Height > 0.3) colour = vec3(0.6, 0.3, 0.3);
    else if (v_Height > 0.2) colour = vec3(0.6, 0.3, 0.3);
    else if (v_Height > 0.0) colour = vec3(0.1, 0.5, 0.8);
    else colour = vec3(0.05, 0.25, 0.4);

    vec3 L = normalize(vec3(c_LightPosition - v_FragPos));
    vec3 N = normalize(v_Normal);
    vec3 V = normalize(u_CameraView - v_FragPos);
    vec3 reflect = reflect(-L, N);

    vec3 ambient = c_AmbientColour * c_Ambient;
    vec3 diffuse = max(dot(L, N), 0.0) * c_Diffuse * c_LightColour;
    vec3 specular = pow(max(dot(V, reflect), 0.0), c_Alpha) * c_Specular * c_LightColour;

    vec3 outputColour = (ambient + diffuse + specular) * colour;

    // f_Colour = vec4(abs(v_Normal.rrb), 1.0);
    f_Colour = vec4(outputColour, 1.0);
}
