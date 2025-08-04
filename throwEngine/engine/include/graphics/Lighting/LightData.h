#pragma once
#include <iostream>
#include <memory>
#include <glm/ext.hpp>
#include <unordered_map>

namespace LIGHTING
{
	class Light;

	class LightData
	{
	public:
		explicit LightData(const glm::vec3& lightPos);

		void calculateDirection(const std::shared_ptr<Light>& light);
		void setPosition(const glm::vec3& pos);

		[[nodiscard]] const glm::vec3& getDirection() const { return m_direction;   };
		[[nodiscard]] const glm::vec3& getPosition()  const { return m_pos;		    };

		void setCutOff(const float cutOff)           { if (m_cutOff != cutOff) { m_cutOff = cutOff; } };
		[[nodiscard]] float getCutOff()	   const     { return m_cutOff;             };

		void setOuterCutOff(const float outerCutOff) { if (m_outerCutOff != outerCutOff) { m_outerCutOff = outerCutOff; } };
		[[nodiscard]] float getOuterCutOff() const   { return m_outerCutOff;        };

		void setConstant(const float constant)       { if (m_constant != constant) { m_constant = constant; } };
		[[nodiscard]] float getConstant()    const   { return m_constant;           };

		void setLinear(const float linear)           { if (m_linear != linear) { m_linear = linear; } };
		[[nodiscard]] float getLinear()	     const   { return m_linear;		        };

		void setQuadratic(const float quadratic)     { if (m_quadratic != quadratic) { m_quadratic = quadratic; } };
		[[nodiscard]] float getQuadratic()   const   { return m_quadratic;          };

		void setDiffuse(const glm::vec3& diffuse)    { if (m_diffuse != diffuse) { m_diffuse = diffuse; } };
		[[nodiscard]] glm::vec3& getDiffuse()        { return m_diffuse;            };

		void setSpecular(const glm::vec3& specular)  { if (m_specular != specular) { m_specular = specular; } };
		[[nodiscard]] glm::vec3& getSpecular()       { return m_specular;           };

	private:
		// position and direction
		glm::vec3 m_pos;
		glm::vec3 m_direction;

		// scene center reference point
		glm::vec3 m_SceneCenter;

		// for scene lighting not light object material properties
		glm::vec3 m_diffuse  = glm::vec3(0.3);
		glm::vec3 m_specular = glm::vec3(0.8);

		// spotlight soft edge
		float m_cutOff		= static_cast<float>(glm::cos(glm::radians(12.5)));
		float m_outerCutOff = static_cast<float>(glm::cos(glm::radians(17.5)));

		// attenuation parameters
		float m_constant  = 1.0f;
		float m_linear	  = 0.014f;
		float m_quadratic = 0.0007f;
	};
}
