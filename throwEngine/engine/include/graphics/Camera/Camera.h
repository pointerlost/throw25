#define GLM_ENABLE_EXPERIMENTAL
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace Graphics
{
	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera
	{
	public:
		Camera();

		void setIsCameraDirty(bool b) { m_isCameraDirty = b; };
		bool getIsCameraDirty() const { return m_isCameraDirty; };

		void setYaw(float y);
		float getYaw() const;

		void setPitch(float p);
		float getPitch() const;

		void setCameraPosition(float speed, glm::vec3 front);
		void setCameraPosition(float speed, glm::vec3 front, glm::vec3 up);
		void setCameraPosition(glm::vec3 pos);
		void setCamFront(float x, float y, float z) { this->cameraFront = glm::vec3{ x,y,z }; };
		
		glm::vec3 getCameraPosition() const;
		glm::vec3 getCamFront() const;
		glm::vec3 getCamUp() const;

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;
		void setViewMatrix(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up);
		void setProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane);

		void updateCameraVectors();
		void updateCamera();

	private:

		bool m_isCameraDirty = false;

		glm::vec3 cameraPos;
		glm::vec3 cameraTarget;
		// we are using cameraFront for camera direction vector! so it is not necessary but we can back again here!
		glm::vec3 cameraDirection; // (in OpenGL) the camera points towards the negative z-axis!
		// so we have to take negative direction for this variable -> -cameraDirection;
		// so u have to use it (cameraPos - cameraTarget) rather than (cameraTarget - cameraPos)!

		glm::vec3 up; // world up vector
		glm::vec3 cameraRight;

		glm::vec3 cameraUp;
		glm::vec3 cameraFront;

		glm::vec3 worldUp;

		float yaw;
		float pitch;

		int SCR_WIDTH = 1280;
		int SCR_HEIGHT = 720;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		void initResources();
	};
}

