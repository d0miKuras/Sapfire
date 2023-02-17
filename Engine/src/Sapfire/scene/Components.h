#pragma once
#include "glm/glm.hpp"
#include "Sapfire/renderer/Mesh.h"

namespace Sapfire
{
	struct TransformComponent
	{
		glm::mat4 Transform{ 0.f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}
	};

	struct MeshRendererComponent
	{
		Mesh Mesh3D;

		MeshRendererComponent() = default;
		MeshRendererComponent(const Mesh& mesh) : Mesh3D(mesh) {}
		MeshRendererComponent(const std::string& path) : Mesh3D(path) {}
	};
}