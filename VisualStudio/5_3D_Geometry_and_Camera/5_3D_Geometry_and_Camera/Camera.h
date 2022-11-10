#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
public:
	//Camera transform
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	//Camera parameters
	float pitch;
	float yaw;
	float speed;
	float fov;

	Camera(glm::vec3 startingPosition, glm::vec3 startingFront)
	{
		position = startingPosition;
		forward = startingFront;

		pitch = 0.0f;
		yaw = -90.0f;
		speed = 1.0f;
		fov = 45.0f; //fov == zoom

		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(position, position + forward, up);
	}

private:
	void UpdateCameraVectors()
	{
		glm::vec3 direction;
		direction.x = glm::cos(glm::radians(yaw));
		direction.y = glm::sin(glm::radians(pitch));
		direction.z = glm::sin(glm::radians(yaw));

		forward = glm::normalize(direction);
		right = glm::normalize(glm::cross(forward, WORLD_UP));
		up = glm::normalize(glm::cross(right, forward));
	}
};

#endif