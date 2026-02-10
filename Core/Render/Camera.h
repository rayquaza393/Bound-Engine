#pragma once

#include "../Math/Vector.h"

namespace Bound {

	class Camera {
	public:
		Camera();
		~Camera();

		// Transform matrices
		Mat4 getViewMatrix() const;
		Mat4 getProjectionMatrix() const;
		Mat4 getViewProjectionMatrix() const;

		// Position and orientation
		void setPosition(const Vec3& pos) { position_ = pos; }
		void setRotation(float yaw, float pitch, float roll) { yaw_ = yaw; pitch_ = pitch; roll_ = roll; }
		
		Vec3 getPosition() const { return position_; }
		Vec3 getForward() const;
		Vec3 getRight() const;
		Vec3 getUp() const;

	// Camera properties
		void setFOV(float fov) { fov_ = fov; }
		void setAspect(float aspect) { aspect_ = aspect; }
		void setNearPlane(float nearPlane) { nearPlane_ = nearPlane; }
		void setFarPlane(float farPlane) { farPlane_ = farPlane; }

		float getFOV() const { return fov_; }
		float getAspect() const { return aspect_; }
		float getNear() const { return nearPlane_; }
		float getFar() const { return farPlane_; }

		// Movement
		void move(const Vec3& offset) { position_ = position_ + offset; }
		void moveForward(float distance);
		void moveRight(float distance);
		void moveUp(float distance);

		// Rotation helpers
		void rotate(float yawDelta, float pitchDelta);

	private:
		Vec3 position_;
		float yaw_;    // Rotation around Y axis (horizontal)
		float pitch_;   // Rotation around X axis (vertical)
		float roll_;    // Rotation around Z axis (usually 0)

		float fov_;         // Field of view in radians
		float aspect_;      // Aspect ratio (width/height)
		float nearPlane_;   // Near clipping plane
		float farPlane_;    // Far clipping plane
	};

}
