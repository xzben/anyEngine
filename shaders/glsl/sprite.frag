#version 450
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec2 vTexcoord;

layout(location = 0) out vec4 FragColor;

layout(set = 0, binding = 0) uniform sampler2D MainTexture;

void main() {
    vec4 color = texture(MainTexture, vTexcoord);
    FragColor =  color;
}