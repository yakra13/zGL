#include "primitiveBuilder.h"

PrimitiveBuilder* PrimitiveBuilder::instance = nullptr;

void PrimitiveBuilder::_TransformVertices(std::vector<Vertex>& vertices,
										  glm::vec3 position,
										  glm::vec3 rotation,
										  glm::vec3 scale)
{
	glm::mat4 identity = glm::mat4(1.0f);

	glm::mat4 translateMatrix = glm::translate(identity, position);

	glm::mat4 rotationYaw   = glm::rotate(identity,
										  glm::radians(rotation.y),
										  glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis

	glm::mat4 rotationPitch = glm::rotate(identity,
										  glm::radians(rotation.x),
										  glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis

	glm::mat4 rotationRoll  = glm::rotate(identity,
										  glm::radians(rotation.z),
										  glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis

	glm::mat4 rotationMatrix = rotationYaw * rotationPitch * rotationRoll;
	
	glm::mat4 scaleMatrix = glm::scale(identity, scale);

	glm::mat4 modelMatrix = translateMatrix * rotationMatrix * scaleMatrix;

	for (auto& v : vertices)
	{
		// Transform vertices positions
		glm::vec4 p = glm::vec4(v.position, 1.0f);
		p = modelMatrix * p;
		v.position = glm::vec3(p);

		// Transform normals rotation only
		glm::vec4 n = glm::vec4(v.normal, 1.0f);
		n = rotationMatrix * n;
		v.normal = glm::vec3(n);
	}
}

PrimitiveBuilder::Primitive PrimitiveBuilder::_BuildPlanePrimitive(float length, float width, glm::vec3 origin, bool flipNormals)
{
	float halfLength = length / 2.0f;
	float halfWidth  = width / 2.0f;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	// Vertex creation order: top left, bottom left, top right, bottom right
	for (int row = 0; row < 2; row++)
	{
		for (int col = 0; col < 2; col++)
		{
			float x = row - halfWidth;
			float z = col - halfLength;
		
			Vertex v;
			v.position = glm::vec3(x - origin.x, 0.0f - origin.y, z - origin.z);
			v.normal   = glm::vec3(0.0f, flipNormals ? -1.0f : 1.0f, 0.0f);
			v.uv       = glm::vec2(static_cast<float>(row), static_cast<float>(col));
		
			vertices.push_back(v);
		}
	}
	// Opengl default winding: counter clock-wise
	if (flipNormals)
		indices = { 0, 2, 1, 2, 3, 1 }; // Clock-wise
	else
		indices = { 0, 1, 2, 2, 1, 3 }; // Counter clock-wise

	return { vertices, indices };
}

Mesh PrimitiveBuilder::BuildPlane(float length, float width, glm::vec3 origin, bool flipNormals)
{
	Primitive p = _BuildPlanePrimitive(length, width, origin, flipNormals);

	return Mesh(p.vertices, p.indices);
}

Mesh PrimitiveBuilder::BuildCube(float length, float width, float height, glm::vec3 origin, bool flipNormals)
{
	float halfLength = length / 2.0f;
	float halfWidth  = width / 2.0f;
	float halfHeight = height / 2.0f;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	
	Primitive top = _BuildPlanePrimitive(length, width, glm::vec3(0.0f), flipNormals);
	Primitive bottom = top;
	Primitive left   = top;
	Primitive right  = top;
	Primitive front  = top;
	Primitive back   = top;

	// Move top face up +Y
	_TransformVertices(top.vertices, glm::vec3(0.0f, halfHeight, 0.0f));
	// Rotate bottom face 180 on z-axis and move down -Y
	_TransformVertices(bottom.vertices, glm::vec3(0.0f, -halfHeight, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f));
	// Rotate left face -90 on z-axis and move left -X
	_TransformVertices(left.vertices, glm::vec3(-halfWidth, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -90.0f));
	// Rotate right face 90 on z-axis and move right +X
	_TransformVertices(right.vertices, glm::vec3(halfWidth, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 90.0f));
	// Rotate front face 90 on x-axis and move foward +Z
	_TransformVertices(front.vertices, glm::vec3(0.0f, 0.0f, halfLength), glm::vec3(90.0f, 0.0f, 0.0f));
	// Rotate back face -90 on x-axis and move back -Z
	_TransformVertices(back.vertices, glm::vec3(0.0f, 0.0f, -halfLength), glm::vec3(-90.0f, 0.0f, 0.0f));

	indices.reserve(36); // 36 total indices, 6 per face
	vertices.reserve(24); // 24 total vertices, 4 per face

	// Pack all indices into a single vector
	indices.insert(indices.end(), top.indices.begin(),    top.indices.end());
	indices.insert(indices.end(), bottom.indices.begin(), bottom.indices.end());
	indices.insert(indices.end(), left.indices.begin(),   left.indices.end());
	indices.insert(indices.end(), right.indices.begin(),  right.indices.end());
	indices.insert(indices.end(), front.indices.begin(),  front.indices.end());
	indices.insert(indices.end(), back.indices.begin(),   back.indices.end());

	// Pack all vertices into a single vector
	vertices.insert(vertices.end(), top.vertices.begin(), top.vertices.end());
	vertices.insert(vertices.end(), bottom.vertices.begin(), bottom.vertices.end());
	vertices.insert(vertices.end(), left.vertices.begin(),   left.vertices.end());
	vertices.insert(vertices.end(), right.vertices.begin(),  right.vertices.end());
	vertices.insert(vertices.end(), front.vertices.begin(),  front.vertices.end());
	vertices.insert(vertices.end(), back.vertices.begin(),   back.vertices.end());

	// Update the indices to match the new indexes of the vertices in the combined vector
	for (size_t i = 0; i < indices.size(); i++)
	{
		indices[i] += 4 * (GLuint)(i / 6);
	}

	// Update each vertex position by the origin offset
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].position -= origin;
	}

	// TODO: all UVs are stacked could lay them out to allow texturing each face seperately

	return Mesh(vertices, indices);
}
