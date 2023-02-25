#include "engpch.h"
#include "Renderer.h"
#include "Sapfire/renderer/Camera.h"
#include "Sapfire/renderer/Mesh.h"
#include "Sapfire/renderer/RenderCommands.h"
#include "Sapfire/renderer/Shader.h"

namespace Sapfire
{
	Renderer::SceneData* Renderer::sSceneData = new SceneData;

	WindowAPI Renderer::sWindowAPI = WindowAPI::GLFW;

	void Renderer::begin_scene(const Camera& camera, const glm::mat4& cameraTransform,
	                           const Ref<UniformBuffer>& uniformBuffer)
	{
		PROFILE_FUNCTION();
		sSceneData->ViewMatrix = glm::inverse(cameraTransform);
		sSceneData->ProjectionMatrix = camera.get_projection_matrix();
		uniformBuffer->set_data(sSceneData);
	}

	void Renderer::end_scene()
	{
		PROFILE_FUNCTION();
	}

	void Renderer::submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader)
	{
		PROFILE_FUNCTION();
		shader->bind();
		vertexArray->bind();
		RenderCommands::draw(vertexArray);
	}

	void Renderer::submit_mesh(const Ref<Mesh>& mesh, const Ref<Shader>& shader)
	{
		PROFILE_FUNCTION();
		shader->bind();
		shader->set_matrix_uniform("uWorldTransform", mesh->get_world_transform());
		mesh->render();
	}

	void Renderer::submit_mesh(const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
		PROFILE_FUNCTION();
		auto& shader = mesh->get_shader();
		shader->bind();
		shader->set_matrix_uniform("uWorldTransform", transform);
		mesh->render();
	}

	void Renderer::submit_mesh(Mesh& mesh, const glm::mat4& transform)
	{
		auto& shader = mesh.get_shader();
		shader->bind();
		shader->set_matrix_uniform("uWorldTransform", transform);
		mesh.render();
	}


	void Renderer::on_window_resize(uint16_t width, uint16_t height)
	{
		PROFILE_FUNCTION();
		RenderCommands::set_viewport(0, 0, width, height);
	}
}
