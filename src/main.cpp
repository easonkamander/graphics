#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>

struct PrcIVec2 {
	int data[2];
};

struct PrcFVec4 {
	float data[4];
};

struct PrcObjDyn {
	float matrix[16];
	float color[4];
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

void getMatrix (float* matrix, float* pos, float* dir, float* scl) {
	matrix[0] = (cosf(dir[0])*cosf(dir[2]) + sinf(dir[0])*sinf(dir[1])*sinf(dir[2]))/scl[0];
	matrix[1] = -cosf(dir[1])*sinf(dir[2])/scl[1];
	matrix[2] = (cosf(dir[0])*sinf(dir[1])*sinf(dir[2]) - cosf(dir[2])*sinf(dir[0]))/scl[2];
	matrix[3] = 0;
	matrix[4] = (cosf(dir[0])*sinf(dir[2]) - cosf(dir[2])*sinf(dir[0])*sinf(dir[1]))/scl[0];
	matrix[5] = cosf(dir[1])*cosf(dir[2])/scl[1];
	matrix[6] = (-cosf(dir[0])*cosf(dir[2])*sinf(dir[1]) - sinf(dir[0])*sinf(dir[2]))/scl[2];
	matrix[7] = 0;
	matrix[8] = cosf(dir[1])*sinf(dir[0])/scl[0];
	matrix[9] = sinf(dir[1])/scl[1];
	matrix[10] = cosf(dir[0])*cosf(dir[1])/scl[2];
	matrix[11] = 0;
	matrix[12] = pos[0]/scl[0];
	matrix[13] = pos[1]/scl[1];
	matrix[14] = pos[2]/scl[2];
	matrix[15] = 0;
}

void swap (PrcObjDyn* lst, int bits, int i, int j) {
	PrcObjDyn temp = lst[i];
	lst[i] = lst[j];
	lst[j] = temp;
	if (std::log2(std::max(i, j)) + 1 < bits) {
		swap(lst, bits, 2*i, 2*j);
		swap(lst, bits, 2*i + 1, 2*j + 1);
	}
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

	windowDim[0] = 1800;
	windowDim[1] = 900;

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

	PrcIVec2* prcsPixStcPoint = new PrcIVec2[windowDim[0]*windowDim[1]];
	PrcFVec4* prcsPixStcArrow = new PrcFVec4[windowDim[0]*windowDim[1]];
	float* prcsPixDynDist = new float[windowDim[0]*windowDim[1]];
	int* prcsPixDynObjLog = new int[windowDim[0]*windowDim[1]];
	int objBits = 6;
	int objSize = std::pow(2, objBits);
	PrcObjDyn* prcsObjDyn = new PrcObjDyn[objSize];
	float objInitPos[3] = {0.f, 0.f, 0.f};
	float objInitDir[3] = {0.f, 0.f, 0.f};
	float objInitScl[3] = {1000.f, 1000.f, 1000.f};
	getMatrix(&prcsObjDyn[0].matrix[0], &objInitPos[0], &objInitDir[0], &objInitScl[0]);
	prcsObjDyn[0].color[0] = 0.15;
	prcsObjDyn[0].color[1] = 0.15;
	prcsObjDyn[0].color[2] = 0.15;
	prcsObjDyn[0].color[3] = 1;
	for (int i = objSize/2; i < objSize; i++) {
		float objNewPos[3] = {20.f*rand()/RAND_MAX - 10, 20.f*rand()/RAND_MAX - 10, 20.f*rand()/RAND_MAX - 10};
		float objNewDir[3] = {0.f, 0.f, 0.f};
		float rad = 0.8*rand()/RAND_MAX + 0.8;
		float objNewScl[3] = {2*rad, rad, rad};
		getMatrix(&prcsObjDyn[i].matrix[0], &objNewPos[0], &objNewDir[0], &objNewScl[0]);
		prcsObjDyn[i].color[0] = 1.0*rand()/RAND_MAX;
		prcsObjDyn[i].color[1] = 1.0*rand()/RAND_MAX;
		prcsObjDyn[i].color[2] = 1.0*rand()/RAND_MAX;
		prcsObjDyn[i].color[3] = 1;
	}
	for (int level = 1; level < objBits; level++) {
		for (int i = std::pow(2, objBits - level - 1); i < std::pow(2, objBits - level); i++) {
			float dist = 1000000.f;
			int indx;
			for (int j = 2*i + 1; j < std::pow(2, objBits - level + 1); j++) {
				float d = std::sqrt(
					std::pow(prcsObjDyn[2*i].matrix[12] - prcsObjDyn[j].matrix[12], 2) +
					std::pow(prcsObjDyn[2*i].matrix[13] - prcsObjDyn[j].matrix[13], 2) +
					std::pow(prcsObjDyn[2*i].matrix[14] - prcsObjDyn[j].matrix[14], 2)
				);
				if (d < dist) {
					dist = d;
					indx = j;
				}
			}
			swap(prcsObjDyn, objBits, 2*i + 1, indx);
			float objAvgPos[3] = {
				(prcsObjDyn[2*i].matrix[12] + prcsObjDyn[2*i + 1].matrix[12]) / 2,
				(prcsObjDyn[2*i].matrix[13] + prcsObjDyn[2*i + 1].matrix[13]) / 2,
				(prcsObjDyn[2*i].matrix[14] + prcsObjDyn[2*i + 1].matrix[14]) / 2
			};
			float objAvgDir[3] = {0.f, 0.f, 0.f};
			std::cout << dist << std::endl;
			std::cout << prcsObjDyn[2*i].matrix[0] << std::endl;
			std::cout << prcsObjDyn[2*i + 1].matrix[0] << std::endl;
			float rad = dist/2 + std::max(prcsObjDyn[2*i].matrix[0], prcsObjDyn[2*i + 1].matrix[0]);
			std::cout << rad << std::endl;
			std::cout << std::endl;
			float objAvgScl[3] = {rad, rad, rad};
			getMatrix(&prcsObjDyn[i].matrix[0], &objAvgPos[0], &objAvgDir[0], &objAvgScl[0]);
		}
	}
	GLuint* prcsBufr = new GLuint[5];
	glGenBuffers(5, prcsBufr);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcIVec2)*windowDim[0]*windowDim[1], prcsPixStcPoint, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, prcsBufr[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcFVec4)*windowDim[0]*windowDim[1], prcsPixStcArrow, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, prcsBufr[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*windowDim[0]*windowDim[1], prcsPixDynDist, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, prcsBufr[2]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[3]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*windowDim[0]*windowDim[1], prcsPixDynObjLog, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, prcsBufr[3]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prcsBufr[4]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PrcObjDyn)*objSize, prcsObjDyn, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, prcsBufr[4]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint initShader = getShader(GL_COMPUTE_SHADER, "shaders/init.comp");
	if (!initShader) {
		glDeleteBuffers(5, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint initProgram = glCreateProgram();
	glAttachShader(initProgram, initShader);
	glLinkProgram(initProgram);
	if (!vldtProgram(initProgram)) {
		glDeleteBuffers(5, prcsBufr);
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
		glDeleteBuffers(5, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	GLuint compProgram = glCreateProgram();
	glAttachShader(compProgram, compShader);
	glLinkProgram(compProgram);
	if (!vldtProgram(compProgram)) {
		glDeleteBuffers(5, prcsBufr);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glUseProgram(compProgram);

	float cameraPos[3] = {0.f, 0.f, -40.f};
	float cameraDir[3] = {0.f, 0.f, 0.f};
	float cameraScl[3] = {1.f, 1.f, 1.f};
	float cameraMat[16];
	GLint ptrCamera = glGetUniformLocation(compProgram, "camera");
	getMatrix(&cameraMat[0], &cameraPos[0], &cameraDir[0], &cameraScl[0]);
	glUniformMatrix4fv(ptrCamera, 1, GL_FALSE, &cameraMat[0]);

	double time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		std::cout << 60 * (glfwGetTime() - time) << std::endl;
		time = glfwGetTime();

		glUseProgram(compProgram);
		glUniformMatrix4fv(ptrCamera, 1, GL_FALSE, &cameraMat[0]);
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
		getMatrix(&cameraMat[0], &cameraPos[0], &cameraDir[0], &cameraScl[0]);

		glfwSwapBuffers(window);
	}

	glDeleteBuffers(5, prcsBufr);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}