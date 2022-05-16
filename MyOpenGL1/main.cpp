#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<cerrno>
#include<stdlib.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/vec3.hpp>

#include "shaderClass.h"
#include "stb_image.h"
#include "camera.h"
void processInput(GLFWwindow* window);
void frameBufferSizeCallback(GLFWwindow* window, int screen_width, int screen_height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 lightPos = glm::vec3(1.2f, 2.0f, 2.0f);
Camera ourCamera(cameraPos);
bool firstClick = true;
float vertices[] = {
	//coordinates		//colours		 //texture
	-0.5f,-0.5f,0.0f,	0.0f,0.0f,1.0f,	 0.0f,0.0f,
	-0.5f,0.5f,0.0f,	0.0f,1.0f,0.0f,	 0.0f,1.0f,
	0.5f,0.5f,0.0f,		1.0f,1.0f,0.0f,	 1.0f,1.0f,
	0.5f,-0.5f,0.0f,	1.0f,0.0f,0.0f,	 1.0f,0.0f
};
float vertices_cube[] = {
	//coordinates			normals				textures
	-0.5f, -0.5f, -0.5f,	0.0f,0.0f,-1.0f,		0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f,0.0f,-1.0f,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f,0.0f,-1.0f,		1.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0.0f,0.0f,-1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f,0.0f,-1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f,0.0f,-1.0f,		0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f,		0.0f,0.0f,1.0f,			0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f,0.0f,1.0f,			1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f,0.0f,1.0f,			1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		0.0f,0.0f,1.0f,			1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f,0.0f,1.0f,			0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0.0f,0.0f,1.0f,			0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f,		-1.0f,0.0f,0.0f,		1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		-1.0f,0.0f,0.0f,		1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,0.0f,0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,0.0f,0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		-1.0f,0.0f,0.0f,		0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		-1.0f,0.0f,0.0f,		1.0f, 0.0f,

	0.5f, 0.5f, 0.5f,		1.0f,0.0f,0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f,0.0f,0.0f,		1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f,0.0f,0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f,0.0f,0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		1.0f,0.0f,0.0f,		0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f,0.0f,0.0f,		1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f,-1.0f,0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0.0f,-1.0f,0.0f,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0.0f,-1.0f,0.0f,		1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f,-1.0f,0.0f,		1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f,-1.0f,0.0f,		0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f,-1.0f,0.0f,		0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,0.0f,		0.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f,0.0f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f,0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f,0.0f,		1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,0.0f,		0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,0.0f,		0.0f, 1.0f
};
glm::vec3 cubepositions[] = {
	glm::vec3(0.0f,0.0f,0.0f),
	glm::vec3(2.0f,5.0f,-15.0f),
	glm::vec3(-1.5f,-2.0f,-2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};
//triangle 1
/*float ex2_vertices0[] = {
	0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	-0.5f,-0.5f,0.0f
};*/
//triangle 2
/*float ex2_vertices1[] = {
	-0.5f,0.5f,0.0f,
	-0.5f,-0.5f,0.0f,
	0.5f,0.5f,0.0f
};*/
int indices[] = {
	0,1,3,
	1,2,3
};
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
int main() {
	glfwInit();

	//initialising opengl

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//initialising window

	GLFWwindow* window = glfwCreateWindow(800, 600, "MyOpenGL1", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//setting callback functions for mouse and scroll input

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwMakeContextCurrent(window);

	//initialising glad

	gladLoadGL();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glEnable(GL_DEPTH_TEST);
	Shader ourShader("default.vert", "default.frag");
	Shader lightShader("lighting.vert", "lighting.frag");
	ourShader.use();
	//initialising uniforms for ourShader
	int modelLoc = glGetUniformLocation(ourShader.ID, "model");
	int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
	int viewLoc = glGetUniformLocation(ourShader.ID, "view");
	int objectColorLoc = glGetUniformLocation(ourShader.ID, "objectColor");
	int lightColorLoc = glGetUniformLocation(ourShader.ID, "lightColor");
	int lightPosLoc = glGetUniformLocation(ourShader.ID, "lightPos");
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(objectColorLoc, 0.0, 1.0, 0.0);
	glUniform3f(lightColorLoc, 1.0, 0.5, 1.0);
	lightShader.use();
	int modelLocLight = glGetUniformLocation(lightShader.ID, "model");
	int projectionLocLight = glGetUniformLocation(lightShader.ID, "projection");
	int viewLocLight = glGetUniformLocation(lightShader.ID, "view");
	//texture code
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	float borderColor[] = { 1.0f,1.0f,0.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	int width, height, nrChannels;
	unsigned char* data = stbi_load("pop_cat.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);*/

	//buffers and array objects
	unsigned int VBO, VAO, lightsVAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &lightsVAO);
	glBindVertexArray(lightsVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);
	float timeValue, greenValue;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		//glfwSetCursorPosCallback(window, mouse_callback);e
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourCamera.update();
		ourShader.use();
		//lightShader.use();
		//float currentFrame = glfwGetTime();
		//deltaTime = currentFrame - lastFrame;
		//lastFrame = currentFrame;
		//cameraSpeed = 2.5 * deltaTime;
		//const float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;

		//view = glm::lookAt(glm::vec3((GLfloat)camX, 0.0f, (GLfloat)camZ), cameraTarget, up);
		view = glm::lookAt(ourCamera.cameraPos, ourCamera.cameraTarget, ourCamera.cameraUp);
		projection = glm::perspective(glm::radians(ourCamera.fov), 800.0f / 600.0f, 0.1f, 100.0f);
		//timeValue = glfwGetTime();
		//greenValue = sin(timeValue / 2.0f) + 0.5f;
		//int vertexLocation = glGetUniformLocation(ourShader.ID, "ourColor");
		//glUniform4f(vertexLocation, 0.0f, greenValue, 0.0f, 1.0f);
		//glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::translate(trans,glm::vec3(1.0f, 0.5f, 0.0f));
		/*trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));*/
		//model = glm::rotate(model, 1.0f/15000.0f*(GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glBindVertexArray(VAO);
		//for loop to gen 10 cubes
		/*for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubepositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/
		//gen 1 cube
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubepositions[0]);
		float angle = 20.0f * 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.use();
		model = glm::translate(model, lightPos);
		model = glm::scale(model,glm::vec3(0.2f));
		glBindVertexArray(lightsVAO);
		glUniformMatrix4fv(modelLocLight, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocLight, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLocLight, 1, GL_FALSE, glm::value_ptr(projection));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	/*Exerceise 2: Create the same 2 triangles using two different VAOs and VBOs for their data*/
	/*GLuint VAOs[2], VBOs[2];
	glGenBuffers(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ex2_vertices0), ex2_vertices0, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ex2_vertices1), ex2_vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		// draw first triangle using the data from the first VAO
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// then we draw the second triangle using the data from the second VAO
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}*/
	/*Exerceise 3: . Create two shader programs where the second program uses a different fragment shader
	that outputs the color yellow; draw both triangles again where one outputs the color yellow*/
	/*GLuint VAOs[2], VBOs[2];
	glGenBuffers(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ex2_vertices0), ex2_vertices0, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ex2_vertices1), ex2_vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glLinkProgram(shaderProgramOrange);
		glUseProgram(shaderProgramOrange);
		// draw first triangle using the data from the first VAO
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glLinkProgram(shaderProgramYellow);
		glUseProgram(shaderProgramYellow);
		// then we draw the second triangle using the data from the second VAO
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------

	}*/
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	ourCamera.processWASD(window);
}
void framebufferSizeCallback(GLFWwindow* window, int screen_width, int screen_height) {
	glViewport(0, 0, screen_width, screen_height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstClick) {
		lastX = xpos;
		lastY = ypos;
		firstClick = false;
	}
	ourCamera.processMouseMovement(xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	ourCamera.processScrollMovement(xoffset, yoffset);
}