#include "mesh.h"
#include "tiny_obj_loader.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::LoadFromFile(std::string path)
{
	// NOTE: 8 floats per Vertex, change this if additional Vertex attributes are introduced
	int stride = 8;

	tinyobj::attrib_t attr;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warning;
	std::string error;

	if (!tinyobj::LoadObj(&attr, &shapes, &materials, &warning, &error, path.c_str()))
	{
		std::cerr << warning << error << std::endl;
	}

	size_t totalSize = attr.vertices.size() + attr.normals.size() + attr.texcoords.size();
	_vertices.reserve(totalSize / stride);

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex v
			{
				// positions
				{
					attr.vertices[index.vertex_index * 3],
					attr.vertices[index.vertex_index * 3 + 1],
					attr.vertices[index.vertex_index * 3 + 2]
				},
				// normals
				{
					attr.normals[index.normal_index * 3],
					attr.normals[index.normal_index * 3 + 1],
					attr.normals[index.normal_index * 3 + 2]
				},
				// uvs
				{
					attr.texcoords[index.texcoord_index * 2],
					attr.texcoords[index.texcoord_index * 2 + 1]
				}
			};

			v.position = _preTransform * glm::vec4(v.position, 1.0f);
			v.normal = glm::normalize(glm::mat3(_preTransform) * v.normal);

			_vertices.emplace_back(v);
		}
	}

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 6));

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// TODO: return false at failure points
	return true;
}

void Mesh::Update(float deltaTime)
{
	// TODO: what update logic is there? skeletal animations?
}

void Mesh::Draw()
{
	// TODO: need material ref here or in model?
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}