//An OpenGL documentation: http://docs.gl/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>

//Vertex Shader -> Defines the positions of the vertexes, called once for each vertex. (x, y)
//Fragment Shader -> "Paints" the pixels with colors, very often called many times more than the vertex shader. (RGBA)

//Tries to compile the source code in the gpu. If successfull returns the id of the shader, otherwise zero.
static unsigned int CompileShader(unsigned int type, const std::string source)
{
	//tells opengl to create the shader and returns the id
	unsigned int id = glCreateShader(type);
	//pointer to the first byte of the source code of the shader
	const char* src = source.c_str();
	//how many shader we want to create
	const int amount = 1;
	//how many characters of the string we want to use
	auto length = nullptr;
	//creates the a single shader sending the address of the source code
	glShaderSource(id, amount, &src, length);

	glCompileShader(id);

	//handle errors on the shader compilation
	{
		//send the result address in order to get back the compilation status
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		
		//if shit happened ...
		if (result == GL_FALSE)
		{
			//get the size of the log inside the gpu
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			//tricks to create a string in the stack not in the heap using length as size
			char* message = (char*)alloca(sizeof(char) * length);

			//writes the message on the buffer
			glGetShaderInfoLog(id, length, &length, message);

			//print it out
			std::cout 
				<< "Shader Compilation Error: " 
				<< (type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader: ")
				<< message << std::endl;

			glDeleteShader(id);

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
	unsigned int program = glCreateProgram();
	//the vertex shader
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	//the fragment shader
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	
		//sheenanigans to make it work, read the documentation
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		//if delete the shaders you lose debugging ability, however it occupays some memory
		glDeleteShader(vs);
		glDeleteShader(fs);
	

	return program;
}

void Start()
{
	//declaring how many buffers we want
	const int amount = 1;

	//declare the buffer, its basically an id or the 
	//address where the buffer is stored in memory
	unsigned int buffer;

	//send the buffer to opengl in order it to allocate the buffer properly
	glGenBuffers(amount, &buffer);

	//tells the opengl how we are gonna use the buffer, this case, as a simple array
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//declare the data we want to put in the buffer
	float trianglePositions[]
	{
		-0.5f, -0.5f,
		0, 0.5f,
		0.5f, -0.5f
	};

	//All of it to only to define a single position of a triangle
	
	//define the size of the buffer (in bytes), check the documentation
	const int size = 6 * sizeof(float); // sizeof(trianglePositions);

	//the attribute we want to define is the positioning of a vertex of a triangle
	const int firstAttributeIndex = 0; //because is the first
	//enables the index
	glEnableVertexAttribArray(firstAttributeIndex);
	const int firstAttributeSizeInBytes = 2; //first for X and second for Y
	auto firstAttributeType = GL_FLOAT; //the type of our attribute
	const bool firstAttributeIsNormalized = GL_FALSE; //its already a float, don't need to be normalized

	//the stride works like an index. Its the offset between each element in bytes. In this case, two floats
	const int firstAttributeStride = sizeof(float) * 2;
	const int firstAttributePointer = 0; //because is the address of the first

	//define the layout of the data or how the gpu will read the data we 
	//provided inside the buffer (some ppl will call it shader)
	glVertexAttribPointer(
		firstAttributeIndex,
		firstAttributeSizeInBytes, 
		firstAttributeType, 
		firstAttributeIsNormalized, 
		firstAttributeStride, 
		firstAttributePointer);

	//reads the shader at the path and returns both vertex and frag
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	std::cout << "VERTEX: \n" << source.VertexSource << std::endl;
	std::cout << "FRAGMENT: \n" << source.FragmentSource << std::endl;

	//sends the stuff to the gpu
	unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);

	//bind the shader
	glUseProgram(program);

	//the usage is a hint to opengl of how we are gonna use the it, check documentation
	auto usage = GL_STATIC_DRAW;
	//finally send the data 
	glBufferData(GL_ARRAY_BUFFER, size, trianglePositions, usage);

	glDeleteProgram(program);
}

void Update()
{
	//type of stuff we wanna draw
	auto type = GL_TRIANGLES;

	//we want to draw from the beginning
	const int begin = 0;

	//all the way up to the end of the array
	const int count = 3;
	
	//a draw call!
	glDrawArrays(type, begin, count);
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
