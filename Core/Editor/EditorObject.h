#pragma once

#include "../Math/Vector.h"
#include "../Render/Mesh.h"

namespace Bound {

	enum class ObjectType {
		Cube,
		Pyramid,
		Floor
	};

	struct EditorObject {
		int id;
		ObjectType type;
		Vec3 position;
		Vec3 rotation;  // Euler angles in radians
		Vec3 scale;
		Vec3 color;
		Mesh mesh;      // The actual geometry
		bool selected;

		EditorObject()
			: id(0), type(ObjectType::Cube), position(0, 0, 0),
			  rotation(0, 0, 0), scale(1, 1, 1), color(1, 1, 1),
			  selected(false) {}

		EditorObject(int _id, ObjectType _type, const Vec3& _pos, const Vec3& _col = Vec3(1, 1, 1))
			: id(_id), type(_type), position(_pos), rotation(0, 0, 0),
			  scale(1, 1, 1), color(_col), selected(false) {}

		// Copy constructor
		EditorObject(const EditorObject& other)
			: id(other.id), type(other.type), position(other.position),
			  rotation(other.rotation), scale(other.scale), color(other.color),
			  mesh(other.mesh), selected(other.selected) {}

		// Assignment operator
		EditorObject& operator=(const EditorObject& other) {
			if (this != &other) {
				id = other.id;
				type = other.type;
				position = other.position;
				rotation = other.rotation;
				scale = other.scale;
				color = other.color;
				mesh = other.mesh;
				selected = other.selected;
			}
			return *this;
		}

		// Generate mesh based on type
		void generateMesh();
		
		// Update mesh color (without regenerating geometry)
		void updateMeshColor();
	};

}
