#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float planetSize;

void main()
{
    // Calculate world position of the vertex
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform normals with transpose of inverse of model (important for non-uniform scale)
    Normal = mat3(transpose(inverse(model))) * aNormal;

    TexCoords = aTexCoords;

    // Transform vertex position to clip space
    gl_Position = projection * view * vec4(planetSize * FragPos, 1.0);
}
