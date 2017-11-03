#version 330

layout(location = 0) in vec4 vertexPosition;
uniform float inputTime;
uniform float randnum;

out vec4 fragmentColor;

void main()
{
    vec4 modifiedVertexPosition = vertexPosition;

    // Insert your code for "Slightly-More Advanced Shaders" here.

    vec4 mod = vec4(cos((1+randnum/1000000)*inputTime+randnum/1000000),sin(inputTime),0,1.f); 
    gl_Position = modifiedVertexPosition + mod;

    //Color
    fragmentColor = vec4(sin(inputTime),1,0,1);
}
