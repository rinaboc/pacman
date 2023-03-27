#version 400 core
out vec4 FragColor;  
in vec3 ourColor;
in vec3 vertexPosition;

uniform float intensity;
  
void main() {

    float dist = distance(vertexPosition.xy, vec2(0, 0));
    float rad;

    if(vertexPosition.y >= 0){
        rad = acos(vertexPosition.x / dist);
    } else {
        rad = -acos(vertexPosition.x / dist);
    }

    vec3 drawColor = ourColor;

    if(dist > 0.5f) {
        discard;
    }

    if(!(rad > 3 * intensity/2 || rad < -3 * intensity/2)){
        discard;
    }
    

    FragColor = vec4(drawColor, 1.0f);
}