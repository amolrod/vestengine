// =============================================================================
// FlatColor Shader - Color sólido sin texturas
// =============================================================================
// Uso: Para debugear geometría, wireframes, UI flat
// Uniforms: u_ViewProjection (mat4), u_Transform (mat4), u_Color (vec4)

#shader vertex
#version 410 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main() {
    FragColor = u_Color;
}
