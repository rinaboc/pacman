#version 400 core
out vec4 FragColor;  
in vec3 ourColor;
in vec3 vertexPosition;
  
void main() {

    vec3 drawColor = vec3(1.0, 0.7, 0.3);
    float dist = distance(vertexPosition.xy, vec2(0, 0));    

    if(dist > 0.3){
        FragColor = vec4(drawColor, (1-dist*1.7));
    } else {
         FragColor = vec4(drawColor, 1.0f);
    }



}