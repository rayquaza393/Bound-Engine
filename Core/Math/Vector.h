#pragma once

#include <cmath>
#include <algorithm>

namespace Bound {

	// Simple 2D vector
	struct Vec2 {
		float x, y;

		Vec2() : x(0.0f), y(0.0f) {}
		Vec2(float x, float y) : x(x), y(y) {}

		Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
		Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
		Vec2 operator*(float s) const { return Vec2(x * s, y * s); }
		Vec2 operator/(float s) const { return Vec2(x / s, y / s); }

		float dot(const Vec2& v) const { return x * v.x + y * v.y; }
		float length() const { return std::sqrt(x * x + y * y); }
		Vec2 normalize() const {
			float len = length();
			return len > 0.0f ? Vec2(x / len, y / len) : Vec2(0, 0);
		}
	};

	// Simple 3D vector
	struct Vec3 {
		float x, y, z;

		Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
		Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
		Vec3 operator-() const { return Vec3(-x, -y, -z); }
		Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
		Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }
		Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }

		float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
		Vec3 cross(const Vec3& v) const {
			return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}
		float length() const { return std::sqrt(x * x + y * y + z * z); }
		Vec3 normalize() const {
			float len = length();
			return len > 0.0f ? Vec3(x / len, y / len, z / len) : Vec3(0, 0, 0);
		}
		float distance(const Vec3& v) const { return (*this - v).length(); }
	};

	// Simple 4D vector (for homogeneous coordinates)
	struct Vec4 {
		float x, y, z, w;

		Vec4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		Vec4(float x, float y, float z, float w = 1.0f) : x(x), y(y), z(z), w(w) {}
		Vec4(const Vec3& v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {}

		Vec4 operator+(const Vec4& v) const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
		Vec4 operator-(const Vec4& v) const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
		Vec4 operator*(float s) const { return Vec4(x * s, y * s, z * s, w * s); }
		Vec4 operator/(float s) const { return Vec4(x / s, y / s, z / s, w / s); }

		float dot(const Vec4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
		float length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
	};

	// 4x4 Matrix
	struct Mat4 {
		float m[16]; // Column-major order

		Mat4() {
			// Identity matrix
			m[0] = 1.0f;  m[4] = 0.0f;  m[8] = 0.0f;  m[12] = 0.0f;
			m[1] = 0.0f;  m[5] = 1.0f;  m[9] = 0.0f;  m[13] = 0.0f;
			m[2] = 0.0f;  m[6] = 0.0f;  m[10] = 1.0f; m[14] = 0.0f;
			m[3] = 0.0f;  m[7] = 0.0f;  m[11] = 0.0f; m[15] = 1.0f;
		}

		static Mat4 identity() { return Mat4(); }

		static Mat4 translation(float x, float y, float z) {
			Mat4 result;
			result.m[12] = x;
			result.m[13] = y;
			result.m[14] = z;
			return result;
		}

		static Mat4 translation(const Vec3& v) {
			return translation(v.x, v.y, v.z);
		}

		static Mat4 scale(float x, float y, float z) {
			Mat4 result;
			result.m[0] = x;
			result.m[5] = y;
			result.m[10] = z;
			return result;
		}

		static Mat4 scale(const Vec3& v) {
			return scale(v.x, v.y, v.z);
		}

		static Mat4 rotationX(float angle) {
			Mat4 result;
			float c = std::cos(angle);
			float s = std::sin(angle);
			result.m[5] = c;   result.m[9] = -s;
			result.m[6] = s;   result.m[10] = c;
			return result;
		}

		static Mat4 rotationY(float angle) {
			Mat4 result;
			float c = std::cos(angle);
			float s = std::sin(angle);
			result.m[0] = c;   result.m[8] = s;
			result.m[2] = -s;  result.m[10] = c;
			return result;
		}

		static Mat4 rotationZ(float angle) {
			Mat4 result;
			float c = std::cos(angle);
			float s = std::sin(angle);
			result.m[0] = c;   result.m[4] = -s;
			result.m[1] = s;   result.m[5] = c;
			return result;
		}

		static Mat4 perspective(float fov, float aspect, float nearPlane, float farPlane) {
			Mat4 result;
			float f = 1.0f / std::tan(fov / 2.0f);
			result.m[0] = f / aspect;
			result.m[5] = f;
			result.m[10] = (farPlane + nearPlane) / (nearPlane - farPlane);
			result.m[11] = -1.0f;
			result.m[14] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
			result.m[15] = 0.0f;
			return result;
		}

		Mat4 operator*(const Mat4& other) const {
			Mat4 result;
			for (int row = 0; row < 4; ++row) {
				for (int col = 0; col < 4; ++col) {
					result.m[col * 4 + row] = 0.0f;
					for (int k = 0; k < 4; ++k) {
						result.m[col * 4 + row] += m[k * 4 + row] * other.m[col * 4 + k];
					}
				}
			}
			return result;
		}

		Vec4 operator*(const Vec4& v) const {
			return Vec4(
				m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
				m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
				m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
				m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w
			);
		}

		Mat4 inverse() const;
	};

	// Utility math functions
	inline float lerp(float a, float b, float t) {
		return a + (b - a) * t;
	}

	inline float clamp(float x, float min_val, float max_val) {
		return (x < min_val) ? min_val : ((x > max_val) ? max_val : x);
	}

	inline int clamp(int x, int min_val, int max_val) {
		return (x < min_val) ? min_val : ((x > max_val) ? max_val : x);
	}

	inline float radians(float degrees) {
		return degrees * 3.14159265359f / 180.0f;
	}

	inline float degrees(float radians) {
		return radians * 180.0f / 3.14159265359f;
	}

}
