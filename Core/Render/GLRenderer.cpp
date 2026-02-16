#include "GLRenderer.h"
#include "Camera.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

namespace Bound {

	// Basic vertex shader - simple color + lighting
	static const char* basicVertexShader = R"(
#version 120

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

attribute vec3 aPosition;
attribute vec3 aColor;
attribute vec3 aNormal;

varying vec3 vColor;
varying vec3 vNormal;
varying vec3 vFragPos;

void main() {
	vFragPos = vec3(uModel * vec4(aPosition, 1.0));
	// Simplified normal transform - assumes uniform scaling
	vNormal = normalize(mat3(uModel) * aNormal);
	vColor = aColor;
	
	gl_Position = uProjection * uView * vec4(vFragPos, 1.0);
}
)";

	// Basic fragment shader - simple unlit color
	static const char* basicFragmentShader = R"(
#version 120

varying vec3 vColor;
varying vec3 vNormal;
varying vec3 vFragPos;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main() {
	// Just output the vertex color with simple lighting
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(uLightPos - vFragPos);
	float brightness = 0.5 + 0.5 * max(dot(norm, lightDir), 0.0);
	
	vec3 result = vColor * brightness;
	gl_FragColor = vec4(result, 1.0);
}
)";

	GLRenderer::GLRenderer() {
		printf("=== GLRenderer initializing ===\n");

		// Set OpenGL clear color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Create camera
		camera_ = std::make_unique<Camera>();
		printf("Camera created\n");

		// Initialize shaders
		initializeShaders();

		printf("=== GLRenderer initialized ===\n");
	}

	GLRenderer::~GLRenderer() {
		shutdown();
	}

	void GLRenderer::initializeShaders() {
		basicShader_ = std::make_unique<Shader>(basicVertexShader, basicFragmentShader);
		printf("Basic shader created\n");
	}

	void GLRenderer::beginFrame() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderer::endFrame() {
		// Post-processing would go here
	}

	void GLRenderer::drawMesh(const Mesh& mesh, const glm::mat4& transform) {
		if (mesh.vertices.empty() || mesh.indices.empty()) {
			return;
		}

		// Cast away const to update GPU if needed
		Mesh* nonConstMesh = const_cast<Mesh*>(&mesh);
		if (nonConstMesh->gpuDirty) {
			nonConstMesh->uploadToGPU();
		}

		// Use shader
		basicShader_->use();

		// Set matrices using GLM
		glm::mat4 view = camera_->getGLMViewMatrix();
		glm::mat4 projection = camera_->getGLMProjectionMatrix();

		basicShader_->setMat4("uModel", transform);
		basicShader_->setMat4("uView", view);
		basicShader_->setMat4("uProjection", projection);

		// Set lighting uniforms
		basicShader_->setVec3("uLightPos", glm::vec3(5.0f, 8.0f, 5.0f));
		basicShader_->setVec3("uViewPos", camera_->getGLMPosition());

		// Draw mesh
		nonConstMesh->draw();
	}

	void GLRenderer::shutdown() {
		basicShader_.reset();
		camera_.reset();
	}

}
