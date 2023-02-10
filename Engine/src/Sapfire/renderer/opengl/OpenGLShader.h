#pragma once
#include <glad/glad.h>
#include "Sapfire/renderer/Shader.h"

namespace Sapfire
{
	class OpenGLShader : public Shader
	{
	public:
		const std::unordered_map<std::string, GLenum> shaderTypes{ {"vertex", GL_VERTEX_SHADER},
									  {"fragment", GL_FRAGMENT_SHADER} };

	public:
		OpenGLShader() = default;
		OpenGLShader(const std::string& path);
		~OpenGLShader();

		// sets the active shader to this
		virtual void Bind() override;
		virtual void SetMatrixUniform(const std::string& name, const glm::mat4& matrix) override;
		virtual void SetVectorUniform(const std::string& name, const glm::vec3& vec) override;
		virtual void SetFloatUniform(const std::string& name, float val) override;
		virtual void SetIntUniform(const std::string& name, int val) override;
		inline virtual const std::string& GetName() const override { return mName; }

	private:
		std::string ParseFile(const std::string& path);
		std::unordered_map<GLenum, std::string> Process(const std::string& source);
		// tries to compile specified shader
		bool CompileShader(const std::unordered_map<GLenum, std::string>& sources);
		// tests if compiled
		bool IsCompiled(GLuint shader);
		// tests if linked
		bool IsValidProgram();
		RendererID mShaderProgram;
		std::string mName;
	};
}