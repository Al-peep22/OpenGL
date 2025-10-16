#include "Renderer/Program.h"
#include <iostream>

namespace neu {

    Program::Program() {
        m_program = glCreateProgram();
    }

    Program::~Program() {
        if (m_program) glDeleteProgram(m_program);
    }

    bool Program::Load(const std::string& /*filename*/) {
        // Optional: For now, we just return false
        return false;
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

    void Program::SetUniform(const std::string& name, const neu::vec2& value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniform2f(loc, value.x, value.y);
    }

    void Program::SetUniform(const std::string& name, const neu::vec3& value) {
        GLint loc = GetUniformLocation(name);
        if (loc != -1) glUniform3f(loc, value.x, value.y, value.z);
    }

    // =============================
    // Get Uniform Location
    // =============================

    GLint Program::GetUniformLocation(const std::string& name) {
        auto it = m_uniformLocations.find(name);
        if (it == m_uniformLocations.end()) {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                std::cerr << "⚠️ Warning: Uniform not found: " << name << std::endl;
            }
            m_uniformLocations[name] = location;
        }
        return m_uniformLocations[name];
    }

}