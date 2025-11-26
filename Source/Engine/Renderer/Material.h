#pragma once
#include <imgui.h>                
#include "../glm/glm.hpp"         
#include "../glm/gtc/type_ptr.hpp"
#include "Resources/Resource.h"
#include "GUI/GUI.h"

namespace neu {
	class Shader;
	class Program;

	class Material : public Resource, GUI {
	public:
		enum class Parameter : uint32_t {
			None		= 0,
			BaseMap		= (1 << 0),
			SpecularMap	= (1 << 1),
			EmissiveMap = (1 << 2),
			NormalMap	= (1 << 3),
			CubeMap		= (1 << 4),
			ShadowMap	= (1 << 5),
		};
	public:
		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();
		void UpdateGui() override;

	public:

		res_t<Program> program;
		res_t<Texture> baseMap;
		glm::vec3 baseColor{ 1,1,1 };

		res_t<Texture> specularMap;
		res_t<Texture> emissiveMap;
		glm::vec3 emissiveColor{ 0,0,0 };
		res_t<Texture> normalMap;
		res_t<Texture> cubeMap;
		res_t<Texture> shadowMap;

		float shininess{ 2 };
		glm::vec2 tiling{ 1,1 };
		glm::vec2 offset{ 0,0 };

		float ior{ 1.3 };

		Parameter parameters = Parameter::None;
	};
}