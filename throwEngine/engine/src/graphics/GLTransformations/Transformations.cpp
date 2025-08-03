#include "graphics/GLTransformations/Transformations.h"


namespace GLgraphics
{

	void Transformations::setPosition(const glm::vec3& pos)
	{
		// Matrix multiplication is not commutative!
		// so we have to care the order of multiplication!
		// glm::mat4 identity = glm::mat4(1.0f);

		// but we update this function so commutative not necessary yet, because we are using directly pos vector.

		position = pos;
	}

	void Transformations::setRotation(float angleInDegrees, const glm::vec3& axis)
	{
		rotationAxis = axis;
		rotationAngle = angleInDegrees;
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), axis);
	}

	void Transformations::setScale(const glm::vec3& scaleVec)
	{
		scale = scaleVec;
	}

	void Transformations::addRotation(float angleInDegrees, const glm::vec3& axis)
	{
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angleInDegrees), axis);
	}

	void Transformations::updateAll(const glm::vec3& angles, const glm::vec3& pos, const glm::vec3& scaleVec)
	{
		if (pos != position) position = pos;
		if (eulerAngles != angles) eulerAngles = angles;
		if (scale != scaleVec) scale = scaleVec;
	}

	void Transformations::resetAll()
	{
		rotationMatrix = glm::mat4(1.0f);
		position = glm::vec3(0.0f);
		rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		scale = glm::vec3(1.0f);
	}


	glm::mat4 Transformations::getModelMatrix() const
	{
		glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

		glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0));
		R = glm::rotate(R, glm::radians(eulerAngles.y), glm::vec3(0, 1, 0));
		R = glm::rotate(R, glm::radians(eulerAngles.z), glm::vec3(0, 0, 1));

		return T * rotationMatrix * S;
	}
}