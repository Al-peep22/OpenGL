#pragma once
#include <imgui.h>                
#include "../glm/glm.hpp"         
#include "../glm/gtc/type_ptr.hpp"
#include "Resources/Resource.h"
#include "GUI.h"

namespace neu {
	class Shader;
	class Program;

	class Material : public Resource, GUI {
	public:
		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();
		void UpdateGui() override;

	public:
		float shininess{ 2 };
		glm::vec2 tiling{ 1,1 };
		glm::vec2 offset{ 0,0 };

		res_t<Program> program;
		res_t<Texture> baseMap;
		glm::vec3 baseColor{ 1,1,1 };
		res_t<Texture> specularMap;

	};
}