#version 330

in vec4 fragmentColor;
out vec4 finalColor;

void main()
{
    //vec4 red = vec4(1.0,0,0,1);
    finalColor = fragmentColor;
}
