#include "Shader.h"
#include "Core/File.h"   
#include "Core/Logger.h" 

namespace neu {

	Shader::~Shader() {
		if (m_shader) {
			glDeleteShader(m_shader);
			LOG_INFO("Shader deleted (ID: {})", m_shader);
		}
	}

	bool Shader::Load(const std::string& filename, GLuint shaderType) {
		std::string source;
		if (!neu::file::ReadTextFile(filename, source)) {
			LOG_WARNING("Failed to read shader file: {}", filename);
			return false;
		}

		const char* source_cstr = source.c_str();

		m_shader = glCreateShader(shaderType);
		glShaderSource(m_shader, 1, &source_cstr, NULL);
		glCompileShader(m_shader);


		GLint success = 0;
		glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			std::string infoLog(512, '\0');
			GLsizei length;
			glGetShaderInfoLog(m_shader, (GLsizei)infoLog.size(), &length, &infoLog[0]);
			infoLog.resize(length);

			LOG_WARNING("Shader compilation failed ({}): {}", filename, infoLog);

			glDeleteShader(m_shader);
			m_shader = 0;
			return false;
		}

		LOG_INFO("Shader compiled successfully: {}", filename);
		return true;
	}

}