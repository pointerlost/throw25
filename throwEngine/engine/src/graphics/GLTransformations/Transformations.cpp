#include "graphics/GLTransformations/Transformations.h"


namespace GLgraphics
{

	void Transformations::setPosition(const glm::vec3& pos)
	{
		// Matrix multiplication is not commutative!
		// so we have to care for the order of multiplication!
		// glm::mat4 identity = glm::mat4(1.0f);

		// but we update this function so commutative not necessary yet, because we are using directly pos vector.

		if (position != pos) {
			position = pos;
			markDirty();
		}
	}

	void Transformations::setRotation(float angleInDegrees, const glm::vec3& axis)
	{
		rotationAxis = axis;
		rotationAngle = angleInDegrees;
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), axis);
		markDirty();
	}

	void Transformations::setScale(const glm::vec3& scale)
	{
		this->scale = scale;
		markDirty();
	}

	void Transformations::addRotation(float angleInDegrees, const glm::vec3& axis)
	{
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angleInDegrees), axis);
		markDirty();
	}

	void Transformations::updateAll(const glm::vec3& angles, const glm::vec3& pos, const glm::vec3& scaleVec)
	{
		bool anyChanged = false;
		if (pos != position) {
			position = pos;
			anyChanged = true;
		}
		if (eulerAngles != angles) {
			eulerAngles = angles;
			anyChanged = true;
		}
		if (scale != scaleVec) {
			scale = scaleVec;
			anyChanged = true;
		}
		if (anyChanged) markDirty();
	}

	void Transformations::resetAll()
	{
		rotationMatrix = glm::mat4(1.0f);
		position = glm::vec3(0.0f);
		rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		rotationAngle = 0.0f;
		scale = glm::vec3(1.0f);
		markDirty();
	}

	const glm::mat4& Transformations::getModelMatrix() const
	{
		if (m_modelMatrixDirty) {
			const glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
			const glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

			glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0));
			R = glm::rotate(R, glm::radians(eulerAngles.y), glm::vec3(0, 1, 0));
			R = glm::rotate(R, glm::radians(eulerAngles.z), glm::vec3(0, 0, 1));

			m_cachedModelMatrix = T * R * S;
			m_modelMatrixDirty = false;
		}
		return m_cachedModelMatrix;
	}

	void Transformations::markDirty() const {
		m_modelMatrixDirty = true;
	}
}
