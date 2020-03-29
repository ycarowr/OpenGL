//An OpenGL documentation: http://docs.gl/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GlLog(#x, __FILE__, __LINE__))


int colorUniformId;
float incrementR = 0.05f;
float rUniform = 0;
unsigned int shader;
unsigned int dataBufferId;
unsigned int indexBuffer;


static void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GlLog(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]: " 
			<< error 
			<< " function: " << function
			<< " file:" << file
			<< " line: " << line
			<< std::endl;
		return false;
	}
	return true;
}

static unsigned int CompileShader(unsigned int type, const std::string source)
{
	unsigned int id;
	GlCall(id= glCreateShader(type));
	const char* src = source.c_str();
	const int amount = 1;
	auto length = nullptr;
	GlCall(glShaderSource(id, amount, &src, length));
	GlCall(glCompileShader(id));

	{
		int result;
		GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(sizeof(char) * length);
			GlCall(glGetShaderInfoLog(id, length, &length, message));
			std::cout 
				<< "Shader Compilation Error: " 
				<< (type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader: ")
				<< message << std::endl;

			GlCall(glDeleteShader(id));
			return 0;
		}
	}

	return id;
}

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

//creates a function to read the file at the path and create the shaders 
static ShaderProgramSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);
	std::string line;
	std::stringstream ss[2];
	enum class ShaderType
	{
		NONE =-1, VERTEX = 0, FRAGMENT =1
	};

	ShaderType index = ShaderType::NONE;
	auto invalid = std::string::npos;
	while (getline(stream, line))
	{
		if (line.find("#shader") != invalid)
		{
			if (line.find("vertex") != invalid)
				index = ShaderType::VERTEX;
			else if(line.find("fragment") != invalid)
				index = ShaderType::FRAGMENT;
		}
		else 
		{
			ss[(int)index] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() }; 
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)//strings because it is the actual source code
{
	unsigned int program;
	GlCall(program = glCreateProgram());
	unsigned int vs;
	GlCall(vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	unsigned int fs;
	GlCall(fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));
	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));
	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));
	return program;
}

void Start()
{
	//----------------- Create Buffers 

	float triangles[] //the data of the buffer
	{
		-0.5f, -0.5f, //0
		0.5, -0.5f,	  //1
		0.5f, 0.5f,   //2
		-0.5, 0.5f,   //3
	};

	unsigned int indexes[] //data of the indexes
	{
		0, 1, 2,
		2, 3, 0
	};

	GlCall(glGenBuffers(1, &dataBufferId));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, dataBufferId));
	GlCall(glBufferData(GL_ARRAY_BUFFER, 4 *2 * sizeof(float), triangles, GL_STATIC_DRAW));

	GlCall(glEnableVertexAttribArray(0));
	GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	GlCall(glGenBuffers(1, &indexBuffer));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexes, GL_STATIC_DRAW));

	//----------------- Create Shader

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	std::cout << "VERTEX: \n" << source.VertexSource << std::endl;
	std::cout << "FRAGMENT: \n" << source.FragmentSource << std::endl;
	shader = CreateShader(source.VertexSource, source.FragmentSource);
	GlCall(glUseProgram(shader));

	//----------------- Uniform

	GlCall(colorUniformId = glGetUniformLocation(shader, "u_Color"));
	ASSERT(colorUniformId != -1);
	GlCall(glUniform4f(colorUniformId, 1.0f, 0.3f, 0.8f, 1.0f));

	//----------------- Unbound everything

	GlCall(glUseProgram(0));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void ChangeIncrement()
{
	if (rUniform > 1.0f)
		incrementR = -0.05f;
	else if(rUniform < 0.0f)
		incrementR = 0.05f;

	rUniform += incrementR;
}

void Update()
{
	GlCall(glUseProgram(shader));
	GlCall(glUniform4f(colorUniformId, rUniform, 0.3f, 0.8f, 1.0f));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, dataBufferId));
	GlCall(glEnableVertexAttribArray(0));
	GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
	ChangeIncrement();
	GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	bool glewInitialization = glewInit();
	if (glewInitialization != GLEW_OK)
		std::cout << "Error, GLEW not Ok!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	
	Start();
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Update();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
