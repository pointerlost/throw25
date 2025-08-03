#pragma once
#include <iostream>
#include "Input/InputContext.h"

namespace GLgraphics { class Transformations; };

/* The purpose of this class is to host operations that will be applied to all objects in common! */
namespace TRANSFORM_INPUT
{
	class TransformInputSystem
	{
	public:
		TransformInputSystem(GLgraphics::Transformations* transform, Input::InputContext context);

		void processInput();

		void setUpDataContext();

	private:
		GLgraphics::Transformations* transformation;
		Input::InputContext dataContext;

	};
}