#pragma once
#include <iostream>

namespace Graphics
{

	class IMesh
	{
	public:
		IMesh() = default;
		virtual ~IMesh() = default;

		virtual uint32_t getVAO() const = 0;

		virtual void drawMeshObject() const = 0;

	protected:

		virtual void SetUpMeshResources() = 0;
	};
}