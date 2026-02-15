#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Bound {

	Camera::Camera()
		: position_(0.0f, 0.0f, 0.0f),
		  yaw_(0.0f),
		  pitch_(0.0f),
		  roll_(0.0f),
		  fov_(radians(45.0f)),
		  aspect_(16.0f / 9.0f),
		  nearPlane_(0.1f),
		  farPlane_(1000.0f) {
	}

	Camera::~Camera() {
	}

	Mat4 Camera::getViewMatrix() const {
		// Build a proper view matrix using forward/right/up vectors
		Vec3 forward = getForward();
		Vec3 right = getRight();
		Vec3 up = getUp();

		// View matrix is the inverse of the camera's transformation matrix
		Mat4 view;
		view.m[0] = right.x;    view.m[4] = right.y;    view.m[8] = right.z;    view.m[12] = -right.dot(position_);
		view.m[1] = up.x;       view.m[5] = up.y;       view.m[9] = up.z;       view.m[13] = -up.dot(position_);
		view.m[2] = -forward.x; view.m[6] = -forward.y; view.m[10] = -forward.z; view.m[14] = forward.dot(position_);
		view.m[3] = 0.0f;       view.m[7] = 0.0f;       view.m[11] = 0.0f;      view.m[15] = 1.0f;

		return view;
	}

	Mat4 Camera::getProjectionMatrix() const {
		return Mat4::perspective(fov_, aspect_, nearPlane_, farPlane_);
	}

	Mat4 Camera::getViewProjectionMatrix() const {
		return getProjectionMatrix() * getViewMatrix();
	}

	Vec3 Camera::getForward() const {
		// Forward vector based on yaw and pitch
		float cosPitch = std::cos(pitch_);
		return Vec3(
			std::sin(yaw_) * cosPitch,
			-std::sin(pitch_),
			-std::cos(yaw_) * cosPitch
		).normalize();
	}

	Vec3 Camera::getRight() const {
		// Right vector perpendicular to forward
		Vec3 forward = getForward();
		Vec3 up(0.0f, 1.0f, 0.0f);
		return forward.cross(up).normalize();
	}

	Vec3 Camera::getUp() const {
		// Up vector perpendicular to both forward and right
		Vec3 forward = getForward();
		Vec3 right = getRight();
		return forward.cross(right).normalize();
	}

	void Camera::moveForward(float distance) {
		position_ = position_ + getForward() * distance;
	}

	void Camera::moveRight(float distance) {
		position_ = position_ + getRight() * distance;
	}

	void Camera::moveUp(float distance) {
		position_ = position_ + Vec3(0.0f, 1.0f, 0.0f) * distance;
	}

	void Camera::rotate(float yawDelta, float pitchDelta) {
		yaw_ += yawDelta;
		pitch_ += pitchDelta;

		// Clamp pitch to avoid gimbal lock
		pitch_ = clamp(pitch_, radians(-89.0f), radians(89.0f));
	}

	// GLM versions for GPU rendering
	glm::mat4 Camera::getGLMViewMatrix() const {
		glm::vec3 pos(position_.x, position_.y, position_.z);
		Vec3 forward = getForward();
		Vec3 up = getUp();
		glm::vec3 center = pos + glm::vec3(forward.x, forward.y, forward.z);
		return glm::lookAt(pos, center, glm::vec3(up.x, up.y, up.z));
	}

	glm::mat4 Camera::getGLMProjectionMatrix() const {
		return glm::perspective(
			fov_,
			aspect_,
			nearPlane_,
			farPlane_
		);
	}

}
