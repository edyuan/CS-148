#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>
#include <fstream>
#include <GL/glew.h>

namespace
{
const int SHADER_ERROR_LOG_SIZE = 500;

bool VerifyShaderCompile(GLuint newShaderObject)
{
    GLint compileStatus;
    OGL_CALL(glGetShaderiv(newShaderObject, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetShaderInfoLog(newShaderObject, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Shader compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}

bool VerifyProgramLink(GLuint shaderProgram)
{
    GLint linkStatus;
    OGL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus));
    if (linkStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetProgramInfoLog(shaderProgram, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Program link compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}
}

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
	time = 0;
    vertexPositions = 
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 0.f, 0.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {1.f, 1.f, 0.f, 1.f},
            // Triangle 2
            {0.f, 0.f, 0.f, 1.f},
            {-1.f, 1.f, 0.f, 1.f},
            {-1.f, 0.f, 0.f, 1.f}
        });
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample1();
}

void Assignment2::SetupCamera()
{
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    Application::HandleInput(key, state, repeat, timestamp, deltaTime);
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
}

void Assignment2::SetupExample1()
{
    // Insert "Load and Compile Shaders" code here.
    
    // Read Strings In
    const std::string vertexfilename = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.vert";
	std::ifstream file(vertexfilename);
	std::string vertstrng((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *c_vertstrng = vertstrng.c_str();

    const std::string fragfilename = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.frag";
    std::ifstream file2(fragfilename);
	std::string fragstrng((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());
    const char *c_fragstrng = fragstrng.c_str();
	

	printf("%s", c_vertstrng);

	printf("%s", c_fragstrng);
    // Vertex Shaders
    GLuint vertexshdr = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragshdr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexshdr,1,&c_vertstrng,NULL);
    glShaderSource(fragshdr,1,&c_fragstrng,NULL);
    glCompileShader(vertexshdr);
    glCompileShader(fragshdr);

	// Create Program
	Assignment2::programobject = glCreateProgram();
	glAttachShader(Assignment2::programobject,vertexshdr);
	glAttachShader(Assignment2::programobject, fragshdr);
	glLinkProgram(Assignment2::programobject);
	//GLint programobject= glCreateProgram();
	//glAttachShader(programobject,vertexshdr);
	//glAttachShader(programobject, fragshdr);
	//glLinkProgram(programobject);

    // Checkpoint 1.
    // Modify this part to contain your vertex shader ID, fragment shader ID, and shader program ID.
    const GLuint vertexShaderId = vertexshdr;
    const GLuint fragmentShaderId = fragshdr;
	const GLuint shaderProgramId = programobject;

    // DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    if (!VerifyShaderCompile(vertexShaderId) || !VerifyShaderCompile(fragmentShaderId) || !VerifyProgramLink(shaderProgramId)) {
        std::cout << "FAILURE: Checkpoint 1 failed." << std::endl;
    } else {
        std::cout << "SUCCESS: Checkpoint 1 completed." << std::endl;
    }

    OGL_CALL(glDetachShader(shaderProgramId, vertexShaderId));
    OGL_CALL(glDeleteShader(vertexShaderId));
    OGL_CALL(glDetachShader(shaderProgramId, fragmentShaderId));
    OGL_CALL(glDeleteShader(fragmentShaderId));
    // FINISH DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION

    // Insert "Setup Buffers" code here.
	//1, 2 Create a vertex array
	glGenVertexArrays(1, &(Assignment2::vao));
	glBindVertexArray(Assignment2::vao);
	//3 Buffer Object is inside vertex array, set aside memory to be used as buffer
	glGenBuffers(1, &(Assignment2::vertexbuffer));
	//4 Tell OpenGL we are going to use previous buffer for following operations
	glBindBuffer(GL_ARRAY_BUFFER,Assignment2::vertexbuffer);

	//5 vertex data currently stored in CPU in vertexPositions, associate with binded buffer
	glBufferData(GL_ARRAY_BUFFER, 96,&(vertexPositions[0]),GL_STATIC_DRAW);
	//6 specifies num of coordinates for each vertex, type of coordinate, etc.
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
	//7 Bind attributes
	glEnableVertexAttribArray(0);

}

void Assignment2::Tick(double deltaTime)
{
    // Insert "Send Buffers to the GPU" and "Slightly-More Advanced Shaders" code here.

	// Increment time
	time += deltaTime;
	

	// Send Buffers to GPU
	glUseProgram(programobject);
	GLuint timeuniform = glGetUniformLocation(programobject,"inputTime");
	glUniform1f(timeuniform, time);
	//Random number
	GLuint random = glGetUniformLocation(programobject, "randnum");
	glUniform1f(random, rand());

	glBindVertexArray(Assignment2::vao);

	//Rotate
	glRotatef(45, 0.0, 0.0, 1.0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

}
