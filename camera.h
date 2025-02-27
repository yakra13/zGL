#pragma once

//#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// TODO: need below?
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL // required for glm/gtx
//#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

enum class CameraType
{
	Free,
	LookAt,
	Arcball
};

// TODO: implementation
enum class CameraViewType
{
	Perspective,
	Orthographic
};

class Camera
{

	private:
		CameraType _type = CameraType::Free;

		float _aspectRatio = 16.0f / 9.0f;
		float _nearPlane = 0.1f;
		float _farPlane = 100.0f;
		float _fov = 1.5708f; // 90 degrees (in radians)
		float _distance = 1.0f;

		glm::vec3 _up      = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 _right   = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 _front   = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 _position = glm::vec3(0.0f);
		glm::vec3 _target   = glm::vec3(0.0f);
		
		glm::quat _orientation = glm::quat(glm::vec3(0.0f));

		// NOTE: multiply order: roll->pitch->yaw | in code => x = yaw * pitch * roll; 
		float _roll  = 0.0f;
		float _pitch = 0.0f;
		float _yaw   = 0.0f;

		glm::mat4 _view = glm::mat4(1.0f);
		glm::mat4 _projection = glm::mat4(1.0f);

		void _UpdateVectors();
	public:
		Camera();
		~Camera();

		glm::vec3& GetPositionRef() { return _position; }
		glm::vec3& GetTargetRef() { return _target; }

		glm::mat4& GetViewRef() { return _view; }
		glm::mat4& GetProjectionRef() { return _projection; }

		void Update();

		void SetRotation(float yaw, float pitch, float roll);
		void SetRotation(glm::vec3 yawPitchRoll);

		void SetPosition(float x, float y, float z);
		void SetPosition(glm::vec3 position);

		void SetDistance(float distance);
};