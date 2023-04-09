#version 400 core
out vec4 FragColor;  
in vec3 ourColor;
in vec3 vertexPosition;

uniform float wave;
uniform vec3 colorMask;
  
void main() {

    float dist = distance(vertexPosition.xy, vec2(0, 0));

    float leftEye = distance(vertexPosition.xy, vec2(-0.4, 0.3));
    float rightEye = distance(vertexPosition.xy, vec2(0.4, 0.3));   

    if(vertexPosition.y >= 0){
        if(dist >= 1){
            discard;
        }

        if(dist >= 0.9 && dist < 1){
            FragColor = vec4(ourColor * colorMask, abs((dist - 1f)/0.1f));
            return;
        }
    }

    if(dist > 1.2 || cos(vertexPosition.x*8 + wave)/8 - 0.8f > vertexPosition.y){
        discard;
    }

    if(leftEye < 0.2 || rightEye < 0.2){
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
        FragColor = vec4(ourColor * colorMask, (vertexPosition.y + 1.5)/2f);
    }

}