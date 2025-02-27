#pragma once

#include "mesh.h"
#include "material.h"

class Model
{
	private:
		std::shared_ptr<Mesh> _mesh;
		std::shared_ptr<Material> _material;

	public:
		Model();
		Model(std::shared_ptr<Mesh> mesh);
		~Model();

		void Update(float deltaTime);
		void Draw();
};

