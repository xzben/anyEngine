#version 450
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texcoord;

layout(location = 0) out vec2 vTexcoord;


layout(std140, set = 0, binding = 0) uniform BuiltinGlobal{
    vec4 cc_time;
    vec4 cc_screenSize;
};

layout(std140, set = 0, binding = 1) uniform BuiltinCamera
{
    mat4 cc_matView;
    mat4 cc_matProj;
    mat4 cc_matInvView;
};

layout(std140, set = 0, binding = 2) uniform BuiltinLocalModel{
    mat4 cc_matModel;
    mat4 cc_matNormal;
    vec4 cc_color;
};

void main() {
    vec4 basePos = cc_matProj*cc_matView*cc_matModel*vec4(a_position, 0.0, 1.0);
    gl_Position = basePos;

    vTexcoord = a_texcoord;
}