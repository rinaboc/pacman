#version 400 core
out vec4 FragColor;  
in vec3 ourColor;
in vec3 vertexPosition;

  
void main() {

    vec3 drawColor = ourColor;
    
    FragColor = vec4(drawColor, 1.0f);
}