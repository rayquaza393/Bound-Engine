#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace Bound {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;

		Vertex() : position(0.0f), color(1.0f, 1.0f, 1.0f), normal(0.0f, 1.0f, 0.0f) {}
		Vertex(const glm::vec3& pos, const glm::vec3& col) 
			: position(pos), color(col), normal(0.0f, 1.0f, 0.0f) {}
		Vertex(const glm::vec3& pos, const glm::vec3& col, const glm::vec3& norm)
			: position(pos), color(col), normal(norm) {}
	};

	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		bool gpuDirty; // True if CPU data changed, needs GPU update

		Mesh() : VAO(0), VBO(0), EBO(0), gpuDirty(true) {}
		~Mesh() { cleanup(); }

		void cleanup() {
			if (EBO) glDeleteBuffers(1, &EBO);
			if (VBO) glDeleteBuffers(1, &VBO);
			if (VAO) glDeleteVertexArrays(1, &VAO);
			EBO = VBO = VAO = 0;
		}

		void uploadToGPU();
		void draw();
	};

}
