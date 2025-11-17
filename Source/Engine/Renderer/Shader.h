#pragma once
#include "Resources/Resource.h"
#include <string>
#include <memory>
#include <glad/glad.h> 

namespace neu {

	class Shader : public Resource {
	public:
		Shader() = default;
		~Shader();

		bool Load(const std::string& filename, GLuint shaderType);

		GLuint GetShader() const { return m_shader; }

		void UpdateGui() override {};

	private:
		GLuint m_shader = 0;
	};

}