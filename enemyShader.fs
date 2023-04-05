#version 400 core
out vec4 FragColor;  
in vec3 ourColor;
in vec3 vertexPosition;

uniform float wave;
  
void main() {

    float dist = distance(vertexPosition.xy, vec2(0, 0));
    float dot = dot(vertexPosition.xy, vec2(0, 1.0));    

    if(dot >= 0 && dist >= 1){
        discard;
    }

    if(dist > 1.2 || cos(vertexPosition.x*8 + wave)/8 - 0.8f > vertexPosition.y){
        discard;
    }

    FragColor = vec4(ourColor, 1.0f);
}