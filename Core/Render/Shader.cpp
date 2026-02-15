#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

namespace Bound {

	Shader::Shader(const char* vertexSrc, const char* fragmentSrc) : program_(0) {
		// Compile vertex shader
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSrc, nullptr);
		glCompileShader(vertex);

		// Check vertex compilation
		int success;
		char infoLog[512];
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			printf("Vertex shader compilation failed: %s\n", infoLog);
			glDeleteShader(vertex);
			return;
		}

		// Compile fragment shader
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSrc, nullptr);
		glCompileShader(fragment);

		// Check fragment compilation
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			printf("Fragment shader compilation failed: %s\n", infoLog);
			glDeleteShader(vertex);
			glDeleteShader(fragment);
			return;
		}

		// Link program
		program_ = glCreateProgram();
		glAttachShader(program_, vertex);
		glAttachShader(program_, fragment);
		glLinkProgram(program_);

		// Check link status
		glGetProgramiv(program_, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program_, 512, nullptr, infoLog);
			printf("Shader program linking failed: %s\n", infoLog);
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		printf("Shader program created: %d\n", program_);
	}

	Shader::~Shader() {
		if (program_) {
			glDeleteProgram(program_);
		}
	}

	void Shader::use() const {
		glUseProgram(program_);
	}

	void Shader::setMat4(const char* name, const glm::mat4& mat) const {
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Shader::setVec3(const char* name, const glm::vec3& vec) const {
		glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
	}

	void Shader::setVec4(const char* name, const glm::vec4& vec) const {
		glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::setFloat(const char* name, float value) const {
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::setInt(const char* name, int value) const {
		glUniform1i(getUniformLocation(name), value);
	}

	int Shader::getUniformLocation(const char* name) const {
		return glGetUniformLocation(program_, name);
	}

}
