#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ctime>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Firework.h"

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

const char *vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform vec4 color;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec4 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"   ourColor = color;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"in vec4 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";


const int FIREWORKS = 2;		   // Number of fireworks


std::vector<glm::vec3> m_Vertexes;
std::vector<unsigned int> indices;



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


static glm::vec3 getPoint(GLfloat u, GLfloat v) {
	//GLfloat r = 0.05f;
	GLfloat pi = glm::pi<GLfloat>();
	GLfloat z = std::cos(pi * u);
	GLfloat x = std::sin(pi * u) * std::cos(2 * pi * v);
	GLfloat y = std::sin(pi * u) * std::sin(2 * pi * v);
	// std::cout << x << "," << y << "," << z << std::endl;
	return glm::vec3(x, y, z);
}


static void createSphere(GLuint Longitude, GLuint Latitude) {
	// Longitude：经线切分个数
	// Latitude：纬线切分个数
	GLfloat lon_step = 1.0f / Longitude;
	GLfloat lat_step = 1.0f / Latitude;
	GLuint offset = 0;
	for (int lat = 0; lat < Latitude; lat++) {  // 纬线u
		for (int lon = 0; lon < Longitude; lon++) { // 经线v
			// 一次构造4个点，两个三角形，
			glm::vec3 point1 = getPoint(lat * lat_step, lon * lon_step);
			glm::vec3 point2 = getPoint((lat + 1) * lat_step, lon * lon_step);
			glm::vec3 point3 = getPoint((lat + 1) * lat_step, (lon + 1) * lon_step);
			glm::vec3 point4 = getPoint(lat * lat_step, (lon + 1) * lon_step);
			m_Vertexes.push_back(point1);
			m_Vertexes.push_back(point2);
			m_Vertexes.push_back(point3);
			m_Vertexes.push_back(point4);
			indices.push_back(0 + offset);
			indices.push_back(3 + offset);
			indices.push_back(2 + offset);
			indices.push_back(0 + offset);
			indices.push_back(2 + offset);
			indices.push_back(1 + offset);
			offset += 4;
		}
	}
	// std::cout<<"offset:" << offset << std::endl;
}



static void setCamera(int shaderProgram)
{
	GLint uniView, uniProjection;

	uniView = glGetUniformLocation(shaderProgram, "view");
	uniProjection = glGetUniformLocation(shaderProgram, "projection");

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(
		glm::vec3(0.0f, 3.0f, -0.99f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projection));
}


Firework fw[FIREWORKS];


int main()
{
	srand(time(NULL));

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	unsigned int VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).


	createSphere(60, 60);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertexes.size() * sizeof(glm::vec3), &m_Vertexes[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	GLint uniModel, uniColor;

	uniModel = glGetUniformLocation(shaderProgram, "model");
	uniColor = glGetUniformLocation(shaderProgram, "color");


	float lastTime = glfwGetTime();
	float dt = 0;


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		dt = glfwGetTime() - lastTime;
		if (dt > .1) dt = .1; //Have some max dt
		lastTime = glfwGetTime();

		for (int loop = 0; loop < FIREWORKS; loop++)
		{
			if (!fw[loop].isLaunched())
			{
				fw[loop].updateFramesUntilLaunch();
			}
			else if (!fw[loop].isExploded())
			{
				GLfloat radius = fw[loop].getRadius();
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, fw[loop].getPosition());
				model = glm::scale(model, glm::vec3(radius, radius, radius));
				glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
				glUniform4f(uniColor, fw[loop].getColor().r, fw[loop].getColor().g, fw[loop].getColor().b, fw[loop].getColor().a);

				setCamera(shaderProgram);

				glDrawElements(
					GL_TRIANGLES,      // mode
					indices.size(),    // count
					GL_UNSIGNED_INT,   // type
					(void*)0           // element array buffer offset
				);

				fw[loop].move(dt);
			}
			else
			{
				for (int pLoop = 0; pLoop < fw[loop].getParticleNum(); pLoop++)
				{
					Particle *ptc = &fw[loop].getParticles()[pLoop];

					if (ptc->getColor().a <= 0.0f)
						continue;

					GLfloat radius = ptc->getRadius();
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, ptc->getPosition());
					model = glm::scale(model, glm::vec3(radius, radius, radius));
					glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
					glUniform4f(uniColor, ptc->getColor().r, ptc->getColor().g, ptc->getColor().b, ptc->getColor().a);

					setCamera(shaderProgram);

					glDrawElements(
						GL_TRIANGLES,      // mode
						indices.size(),    // count
						GL_UNSIGNED_INT,   // type
						(void*)0           // element array buffer offset
					);
				}

				fw[loop].explode(dt);
			}
		}
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
