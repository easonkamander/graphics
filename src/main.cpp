#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>

struct PrcsInit {
	int point[4];
	float arrow[4];
};

struct PrcsIter {
	float dist;
};

struct PrcsItem {
	float circle[4];
};

int windowDim[2];

void resize (GLFWwindow* window, int window_x, int window_y) {
	windowDim[0] = window_x;
	windowDim[1] = window_y;
	glViewport(0, 0, windowDim[0], windowDim[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDim[0], windowDim[1], 0, GL_RGBA, GL_FLOAT, NULL);
	// PrcsInit* prcsDataInit = new PrcsInit[windowDim[0]*windowDim[1]];
	// PrcsIter* prcsDataIter = new PrcsIter[windowDim[0]*windowDim[1]];
	// glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[0]);
	// glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcsInit)*windowDim[0]*windowDim[1], prcsDataInit, GL_STATIC_COPY);
	// glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[1]);
	// glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcsIter)*windowDim[0]*windowDim[1], prcsDataIter, GL_DYNAMIC_COPY);
	// glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	// glUseProgram(initProgram);
	// glDispatchCompute(windowDim[0], windowDim[1], 1);
	// glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	// glClear(GL_COLOR_BUFFER_BIT);
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
	cameraDirMat[0] = cosf(cameraDir[0])*cosf(cameraDir[2]) + sinf(cameraDir[0])*sinf(cameraDir[1])*sinf(cameraDir[2]);
	cameraDirMat[1] = cosf(cameraDir[0])*sinf(cameraDir[2]) - cosf(cameraDir[2])*sinf(cameraDir[0])*sinf(cameraDir[1]);
	cameraDirMat[2] = cosf(cameraDir[1])*sinf(cameraDir[0]);
	cameraDirMat[3] = -cosf(cameraDir[1])*sinf(cameraDir[2]);
	cameraDirMat[4] = cosf(cameraDir[1])*cosf(cameraDir[2]);
	cameraDirMat[5] = sinf(cameraDir[1]);
	cameraDirMat[6] = cosf(cameraDir[0])*sinf(cameraDir[1])*sinf(cameraDir[2]) - cosf(cameraDir[2])*sinf(cameraDir[0]);
	cameraDirMat[7] = -cosf(cameraDir[0])*cosf(cameraDir[2])*sinf(cameraDir[1]) - sinf(cameraDir[0])*sinf(cameraDir[2]);
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

	windowDim[0] = 1280;
	windowDim[1] = 720;

	if (!(window = glfwCreateWindow(windowDim[0], windowDim[1], "Graphics", 0, 0))) {
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

	glViewport(0, 0, windowDim[0], windowDim[1]);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDim[0], windowDim[1], 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLuint vertShader = getShader(GL_VERTEX_SHADER, "shaders/base.vert");
	if (!vertShader) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint fragShader = getShader(GL_FRAGMENT_SHADER, "shaders/base.frag");
	if (!fragShader) {
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

	PrcsInit* prcsDataInit = new PrcsInit[windowDim[0]*windowDim[1]];
	PrcsIter* prcsDataIter = new PrcsIter[windowDim[0]*windowDim[1]];
	PrcsItem* prcsDataItem = new PrcsItem[100];
	for (int i = 0; i < 20; i++) {
		prcsDataItem[i].circle[0] = 20.0*rand()/RAND_MAX - 10;
		prcsDataItem[i].circle[1] = 20.0*rand()/RAND_MAX - 10;
		prcsDataItem[i].circle[2] = 20.0*rand()/RAND_MAX - 10;
		prcsDataItem[i].circle[3] = 2.0*rand()/RAND_MAX;
	}
	std::cout << prcsDataItem[0].circle[0] << std::endl;
	std::cout << prcsDataItem[0].circle[1] << std::endl;
	std::cout << prcsDataItem[0].circle[2] << std::endl;
	std::cout << prcsDataItem[0].circle[3] << std::endl;
	GLuint* prcsBufr = new GLuint[3];
	glGenBuffers(3, prcsBufr);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcsInit)*windowDim[0]*windowDim[1], prcsDataInit, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, prcsBufr[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcsIter)*windowDim[0]*windowDim[1], prcsDataIter, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, prcsBufr[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcsItem)*100, prcsDataItem, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, prcsBufr[2]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint initShader = getShader(GL_COMPUTE_SHADER, "shaders/init.comp");
	if (!initShader) {
		glDeleteBuffers(3, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint initProgram = glCreateProgram();
	glAttachShader(initProgram, initShader);
	glLinkProgram(initProgram);
	if (!vldtProgram(initProgram)) {
		glDeleteBuffers(3, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glUseProgram(initProgram);
	glDispatchCompute(windowDim[0], windowDim[1], 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint compShader = getShader(GL_COMPUTE_SHADER, "shaders/main.comp");
	if (!compShader) {
		glDeleteBuffers(3, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint compProgram = glCreateProgram();
	glAttachShader(compProgram, compShader);
	glLinkProgram(compProgram);
	if (!vldtProgram(compProgram)) {
		glDeleteBuffers(3, prcsBufr);
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

	double time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		std::cout << 60 * (glfwGetTime() - time) << std::endl;
		time = glfwGetTime();

		glUseProgram(compProgram);
		glUniform3fv(ptrCameraPos, 1, &cameraPos[0]);
		getCameraDirMat(&cameraDir[0], &cameraDirMat[0]);
		glUniformMatrix3fv(ptrCameraDir, 1, GL_TRUE, &cameraDirMat[0]);
		glDispatchCompute(windowDim[0], windowDim[1], 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glUseProgram(quadProgram);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
			cameraDir[0] -= 0.01;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
			cameraDir[0] += 0.01;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
			cameraDir[1] += 0.01;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
			cameraDir[1] -= 0.01;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE)) {
			cameraPos[0] += 0.1*cosf(cameraDir[1])*sinf(cameraDir[0]);
			cameraPos[1] += 0.1*sinf(cameraDir[1]);
			cameraPos[2] += 0.1*cosf(cameraDir[1])*cosf(cameraDir[0]);
		}

		glfwSwapBuffers(window);
	}

	glDeleteBuffers(3, prcsBufr);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}