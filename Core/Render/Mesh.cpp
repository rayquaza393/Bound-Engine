#include "Mesh.h"
#include <GL/glew.h>
#include <cstdio>

namespace Bound {

	void Mesh::uploadToGPU() {
		if (!gpuDirty) return;
		if (vertices.empty() || indices.empty()) return;

		// Create VAO if needed
		if (VAO == 0) {
			glGenVertexArrays(1, &VAO);
		}
		glBindVertexArray(VAO);

		// Create/update VBO
		if (VBO == 0) {
			glGenBuffers(1, &VBO);
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

		// Create/update EBO
		if (EBO == 0) {
			glGenBuffers(1, &EBO);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);

		// Set vertex attributes
		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// Normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glBindVertexArray(0);

		gpuDirty = false;
		printf("Mesh uploaded to GPU: %zu vertices, %zu indices\n", vertices.size(), indices.size());
	}

	void Mesh::draw() {
		if (VAO == 0 || indices.empty()) return;
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

}
