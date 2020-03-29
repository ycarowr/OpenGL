#include "HelloTriangle.h"

float vertices[] = 
{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

unsigned int m_VAO;
unsigned int m_VBO;
unsigned int m_ShaderProgram;

void CreateVertexBufferObjects()
{
	int size = 1;
	glGenBuffers(size, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void CreateVertexArrayObjects()
{
	glGenVertexArrays(1, &m_VAO);

	// Initialization code (done once (unless your object frequently changes))
	// 1. bind Vertex Array Object
	glBindVertexArray(m_VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

const char* GetVertexShaderSource()
{
	return "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
}

const char* GetFragmentShaderSource()
{
	return "#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
}

void HelloTriangle::OnStart()
{
	CreateVertexBufferObjects();
	CreateVertexArrayObjects();
	const char* vertexShader = GetVertexShaderSource();
	const char* fragmentShader = GetFragmentShaderSource();
	unsigned int vertexShaderId = CreateShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fragmentShaderId = CreateShader(fragmentShader, GL_FRAGMENT_SHADER);
	m_ShaderProgram = CreateShaderProgram(vertexShaderId, fragmentShaderId);

	int indexOfVertexAttribute = 0;
	int sizeOfVertexAttribute = 3;
	auto typeOfData = GL_FLOAT;
	auto isNormalizedData = GL_FALSE;
	int stride = 3 * sizeof(float);
	auto weirdStuff = (void*)0;

	glVertexAttribPointer(
		indexOfVertexAttribute, 
		sizeOfVertexAttribute, 
		typeOfData, 
		isNormalizedData, stride, weirdStuff);
	glEnableVertexAttribArray(0);
}

//Render without VAO
//void HelloTriangle::OnRender()
//{
//	// 0. copy our vertices array in a buffer for OpenGL to use
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	// 1. then set the vertex attributes pointers
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	// 2. use our shader program when we want to render an object
//	glUseProgram(m_ShaderProgram);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//}

void HelloTriangle::OnRender()
{
	glUseProgram(m_ShaderProgram);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
