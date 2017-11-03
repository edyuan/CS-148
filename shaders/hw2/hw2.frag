#version 330
uniform float inputTime;

in vec4 fragmentColor;
out vec4 fragColor;

void main() 
{
    vec4 finalColor = vec4(sin(3*inputTime),cos(inputTime), sin(0.2*inputTime+0.5), 0);

    // Insert your code for "Slightly-More Advanced Shaders" here.

    fragColor = fragmentColor;
}
