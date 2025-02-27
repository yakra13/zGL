#include "camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	switch (_type)
	{
		case CameraType::LookAt:
			glm::vec3 direction = glm::normalize(_target - _position); // Vector pointing to the target
			glm::vec3 newRight  = glm::normalize(glm::cross(_worldUp, direction)); // Right vector
			glm::vec3 newUp     = glm::cross(direction, newRight); // Up vector

			glm::mat3 rotationMatrix(newRight, newUp, -direction);

			_orientation = glm::quat_cast(rotationMatrix); // Convert rotation matrix to quaternion
			
			_UpdateVectors();
			break;
		case CameraType::Arcball:
			glm::quat pitchQ = glm::angleAxis(glm::radians(_pitch), _right);
			glm::quat yawQ = glm::angleAxis(glm::radians(_yaw), _up); // TODO: _worldUp instead if it acts weird
			glm::quat rollQ = glm::angleAxis(glm::radians(_roll), _front);

			_orientation = glm::normalize(yawQ * pitchQ * rollQ * _orientation);

			_UpdateVectors();
			
			_position = _target - _front * _distance;
			break;
		case CameraType::Free:
		default:
			pitchQ = glm::angleAxis(glm::radians(_pitch), _right);
			yawQ = glm::angleAxis(glm::radians(_yaw), _up); // TODO: _worldUp instead if it acts weird
			rollQ = glm::angleAxis(glm::radians(_roll), _front);

			_orientation = glm::normalize(yawQ * pitchQ * rollQ * _orientation);
			
			_UpdateVectors();
	}

	_view = glm::toMat4(_orientation) * glm::translate(glm::mat4(1.0f), _position);
	_projection = glm::perspective(glm::radians(_fov), _aspectRatio, _nearPlane, _farPlane);
}

void Camera::SetRotation(float yaw, float pitch, float roll)
{
	_yaw   = yaw;
	_pitch = pitch;
	_roll  = roll;
}

void Camera::SetRotation(glm::vec3 yawPitchRoll)
{
	SetRotation(yawPitchRoll.x, yawPitchRoll.y, yawPitchRoll.z);
}

void Camera::SetPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void Camera::SetPosition(glm::vec3 position)
{
	_position = position;
}

void Camera::SetDistance(float distance)
{
	_distance = distance;
}


void Camera::_UpdateVectors()
{
	_front = glm::rotate(_orientation, glm::vec3(0.0f, 0.0f, -1.0f));
	_right = glm::rotate(_orientation, glm::vec3(1.0f, 0.0f, 0.0f));
	_up    = glm::rotate(_orientation, glm::vec3(0.0f, 1.0f, 0.0f));
}
