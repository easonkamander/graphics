#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

struct PrcsInit {
	int point[4];
	float arrow[4];
};

struct PrcsIter {
	float dist;
};

void resize (GLFWwindow* window, int window_x, int window_y) {
	glViewport(0, 0, window_x, window_y);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, window_x, window_y, 0, GL_RGBA, GL_FLOAT, NULL);
}

GLuint getShader (GLenum type, std::string file_path) {
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

void getCameraDirMat (float* cameraDir, float* cameraDirMat) {
	cameraDirMat[0] = cosf(cameraDir[0])*cosf(cameraDir[2])+sinf(cameraDir[0])*sinf(cameraDir[1])*sinf(cameraDir[2]);
	cameraDirMat[1] = cosf(cameraDir[2])*sinf(cameraDir[0])*sinf(cameraDir[1]) - cosf(cameraDir[0])*sinf(cameraDir[2]);
	cameraDirMat[2] = cosf(cameraDir[1])*sinf(cameraDir[0]);
	cameraDirMat[3] = cosf(cameraDir[1])*sinf(cameraDir[2]);
	cameraDirMat[4] = cosf(cameraDir[1])*cosf(cameraDir[2]);
	cameraDirMat[5] = -sinf(cameraDir[1]);
	cameraDirMat[6] = cosf(cameraDir[0])*sinf(cameraDir[1])*sinf(cameraDir[2]) - cosf(cameraDir[2])*sinf(cameraDir[0]);
	cameraDirMat[7] = cosf(cameraDir[0])*cosf(cameraDir[2])*sinf(cameraDir[1]) + sinf(cameraDir[0])*sinf(cameraDir[2]);
	cameraDirMat[8] = cosf(cameraDir[0])*cosf(cameraDir[1]);
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

	int window_x = 1280, window_y = 720;

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

	GLuint vertShader = getShader(GL_VERTEX_SHADER, "shaders/base.vert");
	if(!vertShader) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint fragShader = getShader(GL_FRAGMENT_SHADER, "shaders/base.frag");
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
	glUniform1i(glGetUniformLocation(quadProgram, "tex"), 0);

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

	GLint quadPtrPixel = glGetAttribLocation(quadProgram, "pixel");
	glVertexAttribPointer(quadPtrPixel, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(quadPtrPixel);

	std::cout << sizeof(int) << std::endl;
	std::cout << sizeof(float) << std::endl;
	std::cout << sizeof(PrcsInit) << std::endl;

	PrcsInit* prcsDataInit = new PrcsInit[window_x*window_y];
	PrcsIter* prcsDataIter = new PrcsIter[window_x*window_y];
	GLuint* prcsBufr = new GLuint[2];
	glGenBuffers(2, prcsBufr);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcsInit)*window_x*window_y, prcsDataInit, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, prcsBufr[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcsIter)*window_x*window_y, prcsDataIter, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, prcsBufr[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint initShader = getShader(GL_COMPUTE_SHADER, "shaders/init.comp");
	if (!initShader) {
		glDeleteBuffers(2, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint initProgram = glCreateProgram();
	glAttachShader(initProgram, initShader);
	glLinkProgram(initProgram);
	if (!vldtProgram(initProgram)) {
		glDeleteBuffers(2, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glUseProgram(initProgram);
	glDispatchCompute(window_x, window_y, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint compShader = getShader(GL_COMPUTE_SHADER, "shaders/main.comp");
	if (!compShader) {
		glDeleteBuffers(2, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint compProgram = glCreateProgram();
	glAttachShader(compProgram, compShader);
	glLinkProgram(compProgram);
	if (!vldtProgram(compProgram)) {
		glDeleteBuffers(2, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glUseProgram(compProgram);

	float cameraPos[3] = {0.f, 0.f, 0.f};
	GLint ptrCameraPos = glGetUniformLocation(compProgram, "cameraPos");
	glUniform3fv(ptrCameraPos, 1, &cameraPos[0]);

	float cameraDir[3] = {0.f, 0.f, 0.f};
	float cameraDirMat[9];
	GLint ptrCameraDir = glGetUniformLocation(compProgram, "cameraDir");
	getCameraDirMat(&cameraDir[0], &cameraDirMat[0]);
	glUniformMatrix3fv(ptrCameraDir, 1, GL_TRUE, &cameraDirMat[0]);

	// GLuint compMultiple[][2] = {
	// 	{144, 81},
	// 	{16, 9},
	// 	{4, 3},
	// 	{1, 1}
	// };
	// int compMultipleLen = sizeof(compMultiple)/sizeof(compMultiple[0]);
	// GLint compPtrMultiple = glGetUniformLocation(compProgram, "multiple");
	// glUniform2uiv(compPtrMultiple, 1, &compMultiple[0][0]);

	double time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		std::cout << glfwGetTime() - time << std::endl;
		time = glfwGetTime();

		glUseProgram(compProgram);
		glUniform3fv(ptrCameraPos, 1, &cameraPos[0]);
		getCameraDirMat(&cameraDir[0], &cameraDirMat[0]);
		glUniformMatrix3fv(ptrCameraDir, 1, GL_TRUE, &cameraDirMat[0]);
		glDispatchCompute(window_x, window_y, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(quadProgram);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}

		glfwSwapBuffers(window);
	}

	glDeleteBuffers(2, prcsBufr);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}