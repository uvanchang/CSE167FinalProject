#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

uniform sampler2D tex;
in vec2 fragTexCoord;
out vec4 FragColor;

void main()
{
    FragColor = texture(tex, fragTexCoord);
}
