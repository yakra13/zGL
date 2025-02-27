#include "model.h"

Model::Model()
{
}

Model::Model(std::shared_ptr<Mesh> mesh)
{
	_mesh = mesh;
}

Model::~Model()
{
}

void Model::Update(float deltaTime)
{
	//TODO: 
	//_material->Update(deltaTime);
	_mesh->Update(deltaTime);
}

void Model::Draw()
{
	// TODO: prep material (shader)
	//_material->Bind()
	_mesh->Draw();
}
