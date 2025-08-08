#include "graphics/Camera/Camera.h"


namespace Graphics
{

	Camera::Camera()
	{
		initResources();
	}


	void Camera::initResources()
	{
		// Camera position
		cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
		
		// Yaw is the angle of rotation on the horizontal axis. ( left-right )
		yaw = -90.0f;

		// The up/down angle of the camera on the vertical axis. ( up-down )
		pitch = 0.0f;

		/*
		" The 'up' of the world," meaning the fixed upward direction.
			It is generally the same as "up," but as the camera tilts, these two can differ." 
		*/
		// always should be { 0.0f, 1.0f, 0.0f };
		worldUp = glm::vec3{ 0.0f, 1.0f, 0.0f };

		// The up vector is a directional vector that defines the 'up' direction of camera!
		// but up vector always changing with pitch of camera! ( not like worldUp! ).
		up = glm::vec3(0.0f, 1.0f, 0.0f);

		// Should updatecamera() be called? (flag)
		m_isCameraDirty = true;

		updateCamera();
	}

	void Camera::updateCameraVectors()
	{
		glm::vec3 front = { 0,0,0 };

		// Convert spherical coordinates (yaw and pitch) -> direction vector!

		/*							EXAMPLE
			yaw = -90, pitch = 0	-> front = (0, 0, -1)	-> -Z direction
		
			yaw = 0,   pitch = 0	-> front = (1, 0, 0)	-> +X direction
		*/
		
		// Then we normalize it because the direction vector must be of unit length
		// (i.e. only the direction, not the length).
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);

		// Note: cross(A, B) is a sequential operation.
		// So cross(up, front) != cross(front, up) So the sequence affects the camera's right direction.
		// Finds the right direction of the camera by crossing the direction of view with the top of the world.
		cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));

		// update up vector
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	}

	void Camera::updateCamera()
	{
		// we don't have to calculate when we don't play with camera, so this is provide to us performace optimization!
		if (!m_isCameraDirty)
			return;

		updateCameraVectors();

		// parameters -> ( eye, center, up )
		setViewMatrix(cameraPos, cameraPos + cameraFront, cameraUp);

		// parameters -> ( fov, aspect, nearPlane, farPlane )
		// aspect = width / height ratio
		// This matrix is used to draw the 3D scene in perspective on the screen.
		setProjectionMatrix(60.0f, SCR_WIDTH / SCR_HEIGHT, 0.3f, 100.0f);

		m_isCameraDirty = false;
	}

	void Camera::setViewMatrix(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up)
	{
		this->cameraPos = cameraPos;
		this->cameraTarget = cameraTarget;
		this->up = up;

		// glm::lookAt() translates the world from the camera's point of view.
		viewMatrix = glm::lookAt(cameraPos, cameraTarget, up);
		// view matrix defines the position of the camera in the scene and where it is looking.

		// This matrix affects how the model looks. So the camera doesn't actually move, it shoots the scene in reverse.
	}

	void Camera::setProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane)
	{
		// This matrix converts 3D points in the scene to 2D screen coordinates.
		// So 3D allows us to see space as a flat image. This is where the illusion of perspective begins.
		projectionMatrix = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
	}

	void Camera::setYaw(float y)
	{
		m_isCameraDirty = true;

		yaw += y;
	}

	float Camera::getYaw() const
	{
		return yaw;
	}

	void Camera::setPitch(float p)
	{
		m_isCameraDirty = true;

		// we have to fix it with 89 degree because we have to avoid GIMBAL LOCK(it can be) problem!
		pitch += p;
		if (abs(pitch) > 88.0f) {
			setViewMatrix(cameraPos, cameraPos + cameraFront, worldUp);
		}
		else {
			setViewMatrix(cameraPos, cameraPos + cameraFront, cameraUp);
		}
	}

	float Camera::getPitch() const
	{
		return pitch;
	}

	void Camera::setCameraPosition(float speed, glm::vec3 front)
	{
		m_isCameraDirty = true;

		cameraPos += speed * front;
	}

	void Camera::setCameraPosition(float speed, glm::vec3 front, glm::vec3 up)
	{
		m_isCameraDirty = true;

		cameraPos += glm::normalize(glm::cross(front, up)) * speed;
	}

	void Camera::setCameraPosition(glm::vec3 pos)
	{
		m_isCameraDirty = true;

		this->cameraPos = pos;
	}

	glm::vec3 Camera::getCameraPosition() const
	{
		return cameraPos;
	}

	glm::vec3 Camera::getCamFront() const
	{
		return cameraFront;
	}

	glm::vec3 Camera::getCamUp() const
	{
		return cameraUp;
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return viewMatrix;
	}

	glm::mat4 Camera::getProjectionMatrix() const
	{
		return projectionMatrix;
	}
}