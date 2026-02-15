#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

namespace Bound {

	class Shader {
	public:
		Shader(const char* vertexSrc, const char* fragmentSrc);
		~Shader();

		void use() const;
		void setMat4(const char* name, const glm::mat4& mat) const;
		void setVec3(const char* name, const glm::vec3& vec) const;
		void setVec4(const char* name, const glm::vec4& vec) const;
		void setFloat(const char* name, float value) const;
		void setInt(const char* name, int value) const;

		GLuint getProgram() const { return program_; }

	private:
		GLuint program_;

		int getUniformLocation(const char* name) const;
	};

}
