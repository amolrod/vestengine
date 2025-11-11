// =============================================================================
// Texture Shader - Renderizado con textura
// =============================================================================
// Uso: Objetos con texturas, sprites, UI con imágenes
// Uniforms: u_MVP (mat4), u_Model (mat4), u_Texture (sampler2D)

#shader vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;

uniform mat4 u_MVP;
uniform mat4 u_Model;

out vec2 v_TexCoord;
out vec4 v_Color;
out vec3 v_Normal;

void main() {
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_Normal = mat3(u_Model) * a_Normal;  // Transform normal to world space
    gl_Position = u_MVP * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec4 v_Color;
in vec3 v_Normal;

uniform sampler2D u_Texture;

void main() {
    // Multiplica textura por color del vértice (permite tinting)
    // v_Normal está disponible para futura iluminación
    FragColor = texture(u_Texture, v_TexCoord) * v_Color;
}
