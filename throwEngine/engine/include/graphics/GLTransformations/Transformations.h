#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace GLgraphics
{
	// Forward declaration
	class Shader;
	class Mesh;

	class Transformations
	{
	public:
		Transformations() = default;
		~Transformations() = default;

		// glm::normalize returns a temporary glm::vec3, so don't return with const reference
		glm::vec3 getForward() const { return glm::normalize(glm::vec3(m_cachedModelMatrix[2])); };

		void setRotation(float angleInDegrees, const glm::vec3& axis);
		[[nodiscard]] glm::vec3 getRotationAxis() const { return rotationAxis; };

		void setRotationAngle(float angleInDegrees) { rotationAngle = angleInDegrees; };
		[[nodiscard]] float getRotationAngle() const { return rotationAngle; };

		void setScale(const glm::vec3& scale);
		[[nodiscard]] const glm::vec3& getScale() const { return scale; };

		void addRotation(float angleInDegrees, const glm::vec3& axis);

		void setPosition(const glm::vec3& pos);
		[[nodiscard]] const glm::vec3& getPosition() const { return position; };

		void updateAll(const glm::vec3& angles, const glm::vec3& pos, const glm::vec3& scaleVec);

		void resetAll();

		[[nodiscard]] const glm::mat4& getModelMatrix() const;

		void setEulerAngles(const glm::vec3& angles) { eulerAngles = angles; };
		[[nodiscard]] const glm::vec3& getEulerAngles() const { return eulerAngles; }

		void setPitchYawRoll(float pitch, float yaw, float roll) { eulerAngles = glm::vec3(pitch, yaw, roll); };

	private:

		void markDirty() const;

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		float rotationAngle = 45.0f; // degrees
		glm::vec3 scale = glm::vec3(1.0f);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		glm::vec3 eulerAngles = glm::vec3(0.0f);

		// optimization
		mutable glm::mat4 m_cachedModelMatrix = glm::mat4(1.0f);
		mutable bool m_modelMatrixDirty = true;
	};
}

