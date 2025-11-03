#include "Renderer/Program.h"
#include <iostream>
#include "Program.h"


namespace neu {

    Program::Program() {
        m_program = glCreateProgram();
    }

    Program::~Program() {
        if (m_program) glDeleteProgram(m_program);
    }

    bool Program::Load(const std::string& filename) {
        // load program document
        serial::document_t document;
        if (!serial::Load(filename, document)) {
            LOG_WARNING("Could not load program file: {}", filename);
            return false;
        }

        if (!m_program) { m_program = glCreateProgram(); }

        // get/add vertex shader
        std::string shaderName;
        SERIAL_READ_NAME(document, "vertex_shader", shaderName);
        if (!shaderName.empty()) {
            auto shader = neu::Resources().Get<neu::Shader>(shaderName, GL_VERTEX_SHADER);
            if (!shader) {
                LOG_WARNING("Could not get vertex shader: {}", shaderName);
                glDeleteProgram(m_program);
                m_program = 0;

                return false;
            }
            AttachShader(shader);
        }

        // get/add fragment shader
        SERIAL_READ_NAME(document, "fragment_shader", shaderName);
        if (!shaderName.empty()) {
            auto shader = neu::Resources().Get<neu::Shader>(shaderName, GL_FRAGMENT_SHADER);
            if (!shader) {
                LOG_WARNING("Could not get fragment shader: {}", shaderName);
                glDeleteProgram(m_program);
                m_program = 0;

                return false;
            }
            AttachShader(shader);
        }

        return Link();
    }

    void Program::AttachShader(const std::shared_ptr<Shader>& shader) {
        glAttachShader(m_program, shader->GetShader());
    }

    bool Program::Link() {
        glLinkProgram(m_program);

        GLint success = 0;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success) {
            std::string infoLog(512, '\0');
            GLsizei length;
            glGetProgramInfoLog(m_program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
            infoLog.resize(length);

            std::cerr << "Program link failed: " << infoLog << std::endl;

            glDeleteProgram(m_program);
            m_program = 0;
            return false;
        }

        return true;
    }

    void Program::Use() {
        glUseProgram(m_program);
    }

    // Uniform Setters

    void Program::SetUniform(const std::string& name, float value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniform1f(loc, value);
    }

    void Program::SetUniform(const std::string& name, int value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniform1i(loc, value);
    }

    void Program::SetUniform(const std::string& name, unsigned int value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniform1ui(loc, value);
    }

    void Program::SetUniform(const std::string& name, bool value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniform1i(loc, (int)value);
    }

    void Program::SetUniform(const std::string& name, const glm::vec2& value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniform2f(loc, value.x, value.y);
    }

    void Program::SetUniform(const std::string& name, const glm::vec3& value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniform3f(loc, value.x, value.y, value.z);
    }

    void Program::SetUniform(const std::string& name, const glm::mat3& value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Program::SetUniform(const std::string& name, const glm::mat4& value) {
		GLint loc = GetUniformLocation(name);
		if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    // =============================
    // Get Uniform Location
    // =============================

    GLint Program::GetUniformLocation(const std::string& name) {
        auto it = m_uniformLocations.find(name);
        if (it == m_uniformLocations.end()) {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                std::cerr << "Warning: Uniform not found: " << name << std::endl;
            }
            m_uniformLocations[name] = location;
        }
        return m_uniformLocations[name];
    }

}