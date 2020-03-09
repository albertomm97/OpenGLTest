#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
uniform mat4 u_Transform;


void main() {
    gl_Position = u_Transform * vec4(position, 1.0f);
    v_TexCoord = texCoord;
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord) * u_Color;
    color = texColor;
}