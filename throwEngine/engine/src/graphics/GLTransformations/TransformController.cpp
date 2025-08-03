#include "graphics/GLTransformations/TransformController.h"

#include "graphics/GLTransformations/Transformations.h"

namespace TRANSFORM_INPUT
{
	TransformInputSystem::TransformInputSystem(GLgraphics::Transformations* transform, Input::InputContext context) :
		transformation(transform), dataContext(context)
	{
		setUpDataContext();
	}

	void TransformInputSystem::processInput()
	{
		// this function just doing applyRotating call, but we have to call updateAll
		// we don't have to do changing scaling, or tranlation but we have to update all!

		// updateAll ( float angle, float rx, float ry, float rz, float x, float y, float z, float scaleX, float scaleY, float scaleZ )

	}

	void TransformInputSystem::setUpDataContext()
	{
		/* CALCULATION back later
		dataContext.m_currentTime = glfwGetTime();
		dataContext.m_delta = dataContext.m_currentTime - dataContext.m_lastTime;
		dataContext.m_angle = dataContext.m_delta * dataContext.m_rotationSpeed;
		dataContext.m_lastTime = dataContext.m_currentTime;
		*/
		
		dataContext.m_rotationSpeed = 0.3f;
		dataContext.m_delta = -90.0f;
		dataContext.m_angle = 0.5f;
	}

}