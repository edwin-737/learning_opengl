#pragma once
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
class Camera {
public:
	//angles
	float yaw = -90.0f, pitch = 0.0f;
	//variables for mouse input
	float fov = 45.0f;
	float lastX = 400, lastY = 300;
	float lastFrame = 0;
	float cameraSpeed = 0.05f;
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	bool firstClick = true;
	float deltaTime = 0.0f; // Time between current frame and last frame
	Camera(glm::vec3 CAMPOS) {
		cameraPos = CAMPOS;
		cameraTarget = cameraPos + cameraFront;
	}
	void update() {
		float currentFrame = glfwGetTime();
		deltaTime = (float)currentFrame - (float)lastFrame;
		lastFrame = currentFrame;
		cameraSpeed = 2.5 * deltaTime;
		cameraFront.y = -sin(glm::radians(pitch));
		cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(cameraFront);
		cameraTarget = cameraPos + cameraFront;
	}
	void processWASD(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= cameraSpeed * glm::normalize((glm::cross(cameraFront, cameraUp)));
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += cameraSpeed * glm::normalize((glm::cross(cameraFront, cameraUp)));
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cameraPos.y += cameraSpeed * 1.0f;
		if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cameraPos.y -= cameraSpeed * 1.0f;

	}
	void processMouseMovement(double xpos,double ypos) {
		float xoffset = xpos - lastX;
		float yoffset = ypos - lastY;
		lastX = xpos;
		lastY = ypos;
		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	void processScrollMovement(double xoffset,double yoffset) {
		fov -= yoffset;
		if (fov > 45.0f)
			fov = 45.0f;
		if (fov < 1.0f)
			fov = 1.0f;
	}
};