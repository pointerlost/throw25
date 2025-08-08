#include "graphics/Transformations/Transformations.h"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "core/Logger.h"
#include "Math/Math.h"

glm::mat4 & Transform::getModelMatrix() {
	if (m_modelMatrixDirty) {
		const glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
		const glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

		// ( ROTATION -> CALCULATION WITH EULER ANGLES )
		// glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0));
		// R = glm::rotate(R, glm::radians(eulerAngles.y), glm::vec3(0, 1, 0));
		// R = glm::rotate(R, glm::radians(eulerAngles.z), glm::vec3(0, 0, 1));

		// Convert Euler angles (degrees) to radians
		const glm::vec3 radians = glm::radians(eulerAngles);

		// Create quaternion from Euler angles (pitch = x, yaw = y, roll = z)
		const glm::quat rotationQuat = glm::quat(radians);

		// Convert quaternion to rotation matrix
		const glm::mat4 R = glm::toMat4(rotationQuat);

		m_cachedModelMatrix = T * R * S;
		m_modelMatrixDirty = false;
	}
	return m_cachedModelMatrix;
}

void Transform::addPosition(const glm::vec3 &pos) {
	position += pos;
	markDirty();
}

void Transform::addRotation(const glm::vec3 &angle) {
	setEulerAngles(eulerAngles + angle);
}

void Transform::addScale(glm::vec3 s) {
	scale += s;
	markDirty();
}

void Transform::setPosition(const glm::vec3 &pos) {
	position = pos;
	markDirty();
}

void Transform::setRotation(const glm::vec3 &angle) {
	setEulerAngles(angle);
}

void Transform::setScale(const glm::vec3& s) {
	scale = s;
	markDirty();
}

void Transform::markDirty() {
	m_modelMatrixDirty = true;
}

void Transform::setEulerAngles(const glm::vec3 &angle) {
	eulerAngles = MATH::wrapAngles(angle);
	markDirty();
}
