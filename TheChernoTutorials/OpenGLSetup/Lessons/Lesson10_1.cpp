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
//Vertex Shader -> Defines the positions of the vertexes, called once for each vertex. (x, y)
//Fragment Shader -> "Paints" the pixels with colors, very often called many times more than the vertex shader. (RGBA)

//Tries to compile the source code in the gpu. If successfull returns the id of the shader, otherwise zero.
static unsigned int CompileShader(unsigned int type, const std::string source)
{
	//tells opengl to create the shader and returns the id
	unsigned int id;
	GlCall(id= glCreateShader(type));
	//pointer to the first byte of the source code of the shader
	const char* src = source.c_str();
	//how many shader we want to create
	const int amount = 1;
	//how many characters of the string we want to use
	auto length = nullptr;
	//creates the a single shader sending the address of the source code
	GlCall(glShaderSource(id, amount, &src, length));

	GlCall(glCompileShader(id));

	//handle errors on the shader compilation
	{
		//send the result address in order to get back the compilation status
		int result;
		GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		
		//if shit happened ...
		if (result == GL_FALSE)
		{
			//get the size of the log inside the gpu
			int length;
			GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

			//tricks to create a string in the stack not in the heap using length as size
			char* message = (char*)alloca(sizeof(char) * length);

			//writes the message on the buffer
			GlCall(glGetShaderInfoLog(id, length, &length, message));

			//print it out
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

//holds the source strings 
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

//creates a function to read the file at the path and create the shaders 
static ShaderProgramSource ParseShader(const std::string& filePath)
{
	//opens the file
	std::ifstream stream(filePath);

	//current line
	std::string line;

	//shader strings
	std::stringstream ss[2];

	//define an index for stringstream array
	enum class ShaderType
	{
		NONE =-1, VERTEX = 0, FRAGMENT =1
	};

	ShaderType index = ShaderType::NONE;
	
	auto invalid = std::string::npos;

	//iterates the file line by line 
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
			//adds the line to the buffer
			ss[(int)index] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() }; 
}

//Creates the shader program in the gpu with the provided source codes
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)//strings because it is the actual source code
{
	//adress of the program
	unsigned int program;
	GlCall(program = glCreateProgram());

	//the vertex shader
	unsigned int vs;
	GlCall(vs = CompileShader(GL_VERTEX_SHADER, vertexShader));

	//the fragment shader
	unsigned int fs;
	GlCall(fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

	
	//sheenanigans to make it work, read the documentation
	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	//if delete the shaders you lose debugging ability, however it occupays some memory
	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));
	
	return program;
}

void Start()
{
	//declaring how many buffers we want
	const int amount = 1;

	//declare the buffer, its basically an id or the 
	//address where the buffer is stored in memory
	unsigned int dataBuffer;

	//send the buffer to gpu in order it to allocate the buffer properly
	GlCall(glGenBuffers(amount, &dataBuffer));

	//tells the opengl how we are gonna use the buffer, this case, as a simple array
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, dataBuffer));

	//declare the data we want to put in the buffer
	float triagles[]
	{
		-0.5f, -0.5f, //0
		0.5, -0.5f,	  //1
		0.5f, 0.5f,   //2
		-0.5, 0.5f,   //3
	};

	//declare data of index buffer
	unsigned int indexes[]
	{
		0, 1, 2,
		2, 3, 0
	};

	//declare the index buffer
	unsigned int indexBuffer;

	//send the buffer to gpu in order it to allocate the it 
	GlCall(glGenBuffers(amount, &indexBuffer));

	//tells the gpu how we are gonna use the buffer, this case, as an element
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));

	//sent it to the gpu
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexes, GL_STATIC_DRAW));

	//All of it to only to define a single position of a triangle
	
	//define the size of the buffer (in bytes), check the documentation
	const int size = 12 * sizeof(float); // sizeof(trianglePositions);

	//the attribute we want to define is the positioning of a vertex of a triangle
	const int firstAttributeIndex = 0; //because is the first
	//enables the index
	GlCall(glEnableVertexAttribArray(firstAttributeIndex));
	const int firstAttributeSizeInBytes = 2; //first for X and second for Y
	auto firstAttributeType = GL_FLOAT; //the type of our attribute
	const bool firstAttributeIsNormalized = GL_FALSE; //its already a float, don't need to be normalized

	//the stride works like an index. Its the offset between each element in bytes. In this case, two floats
	const int firstAttributeStride = sizeof(float) * 2;
	const int firstAttributePointer = 0; //because is the address of the first

	//define the layout of the data or how the gpu will read the data we 
	//provided inside the buffer (some ppl will call it shader)
	GlCall(glVertexAttribPointer(
		firstAttributeIndex,
		firstAttributeSizeInBytes, 
		firstAttributeType, 
		firstAttributeIsNormalized, 
		firstAttributeStride, 
		firstAttributePointer));

	//reads the shader at the path and returns both vertex and frag
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	std::cout << "VERTEX: \n" << source.VertexSource << std::endl;
	std::cout << "FRAGMENT: \n" << source.FragmentSource << std::endl;

	//sends the stuff to the gpu
	unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);

	//bind the shader
	GlCall(glUseProgram(program));

	//the usage is a hint to opengl of how we are gonna use the it, check documentation
	auto usage = GL_STATIC_DRAW;
	//finally send the data 
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, triagles, usage));

	GlCall(glDeleteProgram(program));
}

void Update()
{
	//type of stuff we wanna draw
	auto mode = GL_TRIANGLES;

	//we want to draw from the beginning
	const int begin = 0;

	//all the way up to the end of the array
	const int count = 6;
	
	//a draw call!
	GlCall(glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr));
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	bool glewInitialization = glewInit();
	if (glewInitialization != GLEW_OK)
		std::cout << "Error, GLEW not Ok!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	//before loop
	Start();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		Update();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}
