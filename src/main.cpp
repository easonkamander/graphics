#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>

GLuint initShader (GLenum type, std::string file_path) {
	std::ifstream file_obj(file_path);
	if (!file_obj) {
		std::cout << "Unable To Read File: " + file_path << std::endl;
		return 0;
	}
	
	std::ostringstream file_buffer;
	file_buffer << file_obj.rdbuf();
	std::string file_data = file_buffer.str();
	const char* file_str = file_data.c_str();
	int file_len = file_data.size();
	
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &file_str, &file_len);
	glCompileShader(shader);
	
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint err_len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &err_len);
		char err_log[err_len];
		glGetShaderInfoLog(shader, err_len, &err_len, err_log);
		std::cerr << err_log;
		return 0;
	}
	
	return shader;
}

bool vldtProgram (GLuint program) {
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint err_len;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &err_len);
		char err_log[err_len];
		glGetShaderInfoLog(program, err_len, &err_len, err_log);
		std::cerr << err_log;
		return false;
	}
	return true;
}

int window_x, window_y;

float cameraDir[3][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0}
};

void setCameraDir (float a, float b, float c) {
	cameraDir[0][0] = cosf(a)*cosf(c)+sinf(a)*sinf(b)*sinf(c);
	cameraDir[1][0] = cosf(c)*sinf(a)*sinf(b) - cosf(a)*sinf(c);
	cameraDir[2][0] = cosf(b)*sinf(a);
	cameraDir[0][1] = cosf(b)*sinf(c);
	cameraDir[1][1] = cosf(b)*cosf(c);
	cameraDir[2][1] = -sinf(b);
	cameraDir[0][2] = cosf(a)*sinf(b)*sinf(c) - cosf(c)*sinf(a);
	cameraDir[1][2] = cosf(a)*cosf(c)*sinf(b) + sinf(a)*sinf(c);
	cameraDir[2][2] = cosf(a)*cosf(b);
}

void resize (GLFWwindow* window, int width, int height) {
	window_x = width, window_y = height;
	glViewport(0, 0, window_x, window_y);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, window_x, window_y, 0, GL_RGBA, GL_FLOAT, NULL);
}

int main () {
	if (!glfwInit()) {
		std::cerr << "Init Error" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* window;

	window_x = 1280, window_y = 720;

	if (!(window = glfwCreateWindow(window_x, window_y, "Graphics", 0, 0))) {
		std::cerr << "Window Error" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwSetFramebufferSizeCallback(window, resize);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "GLAD Error" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, window_x, window_y);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, window_x, window_y, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLuint vertShader = initShader(GL_VERTEX_SHADER, "shaders/base.vert");
	if(!vertShader) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint fragShader = initShader(GL_FRAGMENT_SHADER, "shaders/base.frag");
	if(!fragShader) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint quadProgram = glCreateProgram();
	glAttachShader(quadProgram, vertShader);
	glAttachShader(quadProgram, fragShader);
	glBindFragDataLocation(quadProgram, 0, "color");
	glLinkProgram(quadProgram);
	if (!vldtProgram(quadProgram)) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glUseProgram(quadProgram);
	glUniform1i(glGetUniformLocation(quadProgram, "srcTex"), 0);

	GLuint quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	GLuint quadBuffer;
	glGenBuffers(1, &quadBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);

	float quadPoints[8] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, quadPoints, GL_STREAM_DRAW);

	GLint quadPtrPix = glGetAttribLocation(quadProgram, "pixel");
	glVertexAttribPointer(quadPtrPix, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(quadPtrPix);

	float savedLengthData [2*window_x*window_y] = {0.f};

	GLuint ssboSavedLengths;
	glGenBuffers(1, &ssboSavedLengths);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboSavedLengths);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(savedLengthData), savedLengthData, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboSavedLengths);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint compShader = initShader(GL_COMPUTE_SHADER, "shaders/main.comp");
	if (!compShader) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint compProgram = glCreateProgram();
	glAttachShader(compProgram, compShader);
	glLinkProgram(compProgram);
	if (!vldtProgram(compProgram)) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}
	
	glUseProgram(compProgram);

	GLint compPtrStage = glGetUniformLocation(compProgram, "stage");
	GLuint compStageRst = 0;
	GLuint compStageAdv = 1;
	GLuint compStageClr = 2;
	glUniform1ui(compPtrStage, compStageRst);

	GLuint compMultiple[][2] = {
		{144, 81},
		{16, 9},
		{4, 3},
		{1, 1}
	};
	int compMultipleLen = sizeof(compMultiple)/sizeof(compMultiple[0]);
	GLint compPtrMultiple = glGetUniformLocation(compProgram, "multiple");
	glUniform2uiv(compPtrMultiple, 1, &compMultiple[0][0]);

	float compCameraPosVect[3] = {
		0.f, 0.f, 0.f
	};
	GLint compPtrCameraPos = glGetUniformLocation(compProgram, "cameraPos");
	glUniform3fv(compPtrCameraPos, 1, &compCameraPosVect[0]);

	float compCameraDirVect[3] = {
		0.f, 0.f, 0.f
	};
	GLint compPtrCameraDir = glGetUniformLocation(compProgram, "cameraDir");
	setCameraDir(0.f, 0.f, 0.f);
	glUniformMatrix3fv(compPtrCameraDir, 1, GL_FALSE, &cameraDir[0][0]);

	double time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		std::cout << glfwGetTime() - time << std::endl;
		time = glfwGetTime();

		for (int i = 0; i < compMultipleLen; i++) {
			glUseProgram(compProgram);
			glUniform1ui(compPtrStage, i == 0 ? compStageRst : (i + 1 == compMultipleLen ? compStageClr : compStageAdv));
			glUniform2uiv(compPtrMultiple, 1, &compMultiple[i][0]);
			glDispatchCompute((GLuint)(window_x - 1)/compMultiple[i][0] + 1, (GLuint)(window_y - 1)/compMultiple[i][1] + 1, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		glUseProgram(quadProgram);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE)) {
			compCameraPosVect[2] += 0.03;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
			compCameraDirVect[0] -= 0.01;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
			compCameraDirVect[0] += 0.01;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
			compCameraDirVect[1] += 0.01;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
			compCameraDirVect[1] -= 0.01;
		}

		glfwSwapBuffers(window);

		glUseProgram(compProgram);
		glUniform3fv(compPtrCameraPos, 1, &compCameraPosVect[0]);
		setCameraDir(compCameraDirVect[0], compCameraDirVect[1], compCameraDirVect[2]);
		glUniformMatrix3fv(compPtrCameraDir, 1, GL_FALSE, &cameraDir[0][0]);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}