#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/bit.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"
#include "vertex.h"

class PrimitiveBuilder
{
	enum class Winding
	{
		CCW,
		CW
	};

	struct Primitive
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
	};

	//struct PlaneHash 
	//{
	//	template <typename T>
	//	size_t operator()(const T& t) const 
	//	{
	//		return std::hash<std::string>()(std::to_string(std::get<0>(t)) + "," +
	//										std::to_string(std::get<1>(t)) + "," +
	//										std::to_string(std::get<2>(t)) + "," + 
	//										std::to_string(std::get<3>(t)));
	//	}
	//};

	private:
	//	std::unordered_map<
		/*std::unordered_map<std::tuple<float, float, int, int>, Primitive, PlaneHash> primitiveCache;
		Primitive _BuildPrimitivePlane(float length, float width, int lengthSegments, int widthSegments, Winding winding);*/
		static PrimitiveBuilder* instance;
		
		void _TransformVertices(std::vector<Vertex>& vertices,
								glm::vec3 position,
								glm::vec3 rotation = glm::vec3(0.0f),
								glm::vec3 scale = glm::vec3(1.0f));
		
		Primitive _BuildPlanePrimitive(float length = 1.0f,
									   float width  = 1.0f,
									   glm::vec3 origin = glm::vec3(0.0f),
									   bool flipNormals = false);
	public:
		PrimitiveBuilder()
		{
			// DEBUG
			if (instance == nullptr)
			{
				std::cout << "Construct PrimitiveBuilder" << std::endl;
				instance = this;

			}
		}

		~PrimitiveBuilder() { std::cout << "Destruct PrimitiveBuilder" << std::endl; }

		// Delete the copy constructor
		PrimitiveBuilder(const PrimitiveBuilder&) = delete;
		// Delete assignment operator
		PrimitiveBuilder& operator=(const PrimitiveBuilder&) = delete;

		Mesh BuildPlane(float length = 1.0f,
						float width = 1.0f,
						glm::vec3 origin = glm::vec3(0.0f),
						bool flipNormals = false);

		/// <summary>
		/// Build a cube's vertices and indices.
		/// </summary>
		/// <param name="length">Cube length along the Z-axis</param>
		/// <param name="width">Cube width along the X-axis</param>
		/// <param name="height">Cube height along the Y-axis</param>
		/// <param name="origin">Offset the cube from the world origin.</param>
		/// <param name="flipNormals">If true the cube will be rendered inside-out.</param>
		/// <returns>Mesh object containing the vertices and indices of the cube.</returns>
		Mesh BuildCube(float length = 1.0f,
					   float width  = 1.0f, 
			           float height = 1.0f,
					   glm::vec3 origin = glm::vec3(0.0f),
					   bool flipNormals = false);


		//Mesh BuildPlane(float length, float width, int lengthSegments, int widthSegments);
		//Mesh BuildCube(float length, float width, float height, int lengthSegments, int widthSegments, int heightSegments, Winding winding);
};
