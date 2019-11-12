#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 Normal;
in vec3 FragPos;

uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform int normalColoring;
uniform int reflection;
uniform vec3 color;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 FragColor;

void main()
{	
	if (normalColoring > 0) // Use normal coloring
	{
		vec3 norm = normalize(Normal);
		FragColor = vec4(norm, 1.0f);
	}
	else if (reflection > 0) // Use environment mapping
	{
	    vec3 I = normalize(FragPos - cameraPos);
	    vec3 R = reflect(I, normalize(Normal));
	    FragColor = vec4(texture(skybox, R).rgb, 1.0);
	}
	else
	{
		FragColor = vec4(color, 1.0f);
	} 
}