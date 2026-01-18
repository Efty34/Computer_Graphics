#version 330 core
out vec4 FragColor;

uniform vec3 customColor;
uniform bool lightOn;

void main()
{
    vec3 resultColor = customColor;
    
    if(!lightOn) {
        // light off,dim the color
        resultColor = resultColor * 0.2; 
    }
    
    FragColor = vec4(resultColor, 1.0f);
}