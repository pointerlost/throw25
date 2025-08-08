#include "Input/InputComponentFactory.h"

#include <Input/InputComponent.h>

#include <core/Logger.h>

#include <graphics/Transformations/Transformations.h>

#include <graphics/Lighting/Light.h>

namespace Input
{
	std::shared_ptr<IInputComponent> InputComponentFactory::createObjectComponent(InputType type, std::shared_ptr<Graphics::Transform> transform)
	{
		Input::InputContext context{};

		switch (type)
		{
		case InputType::CubeInputComponent:
			return std::make_shared<CubeInputComponent>(transform, context);
		case InputType::SphereInputComponent:
			return std::make_shared<SphereInputComponent>(transform, context);
		default:
			Logger::error("Unknown input component type: " + std::to_string(static_cast<int>(type)));
			return nullptr;
		}
	}

	std::shared_ptr<IInputComponent> InputComponentFactory::createLightComponent(
		InputType type,
		std::shared_ptr<Graphics::Transform> transform,
		std::shared_ptr<LIGHTING::Light>& light)
	{
		Input::InputContext context{};
	
		return std::make_shared<LightInputComponent>(transform, context, light);
	}
} // namespace Input