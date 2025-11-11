// =============================================================================
// Texture Shader - Renderizado con textura
// =============================================================================
// Uso: Objetos con texturas, sprites, UI con imágenes
// Uniforms: u_ViewProjection (mat4), u_Transform (mat4), u_Texture (sampler2D)

#shader vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {
    FragColor = texture(u_Texture, v_TexCoord);
}
