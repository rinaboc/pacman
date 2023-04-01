#version 400 core
out vec4 FragColor;  
in vec3 ourColor;
in vec3 vertexPosition;

uniform float intensity;
  
void main() {
    FragColor = vec4(ourColor, 1.0f);
}
