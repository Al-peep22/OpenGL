#pragma once
#include <string>
#include <map>
#include <memory>
#include <glad/glad.h>
#include "Renderer/Shader.h" 
#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace neu {


    class Shader;

    class Program : public Resource {
    public:
        Program();
        ~Program();

        bool Load(const std::string& filename);
        void AttachShader(const std::shared_ptr<Shader>& shader);
        bool Link();
        void Use();

        // Uniforms
        void SetUniform(const std::string& name, float value);
        void SetUniform(const std::string& name, int value);
        void SetUniform(const std::string& name, unsigned int value);
        void SetUniform(const std::string& name, bool value);

        void SetUniform(const std::string& name, const neu::vec2& value);
        void SetUniform(const std::string& name, const neu::vec3& value);

    private:
        GLint GetUniformLocation(const std::string& name);

    public:
        GLuint m_program = 0;
        std::map<std::string, GLint> m_uniformLocations;
    };
}