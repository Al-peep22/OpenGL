#include "Material.h"      
#include "Program.h"

namespace neu {

	bool Material::Load(const std::string& filename) {
		neu::serial::document_t document;
		if (!neu::serial::Load(filename, document)) {
			LOG_ERROR("Could not load Material json {}", filename);
			return false;
		}
		std::string programName;
		SERIAL_READ_NAME(document, "program", programName);

		program = Resources().Get<Program>(programName);

		//texture
		std::string textureName;
		SERIAL_READ_NAME(document, "baseMap", textureName);
		if (!textureName.empty()) baseMap = Resources().Get<Texture>(textureName);

		textureName = "";
		SERIAL_READ_NAME(document, "specularMap", textureName);
		if (!textureName.empty()) specularMap = Resources().Get<Texture>(textureName);

		textureName = "";
		SERIAL_READ_NAME(document, "emissiveMap", textureName);
		if (!textureName.empty()) emissiveMap = Resources().Get<Texture>(textureName);

		textureName = "";
		SERIAL_READ_NAME(document, "normalMap", textureName);
		if (!textureName.empty()) normalMap = Resources().Get<Texture>(textureName);

		textureName = "";
		SERIAL_READ_NAME(document, "cubeMap", textureName);
		if (!textureName.empty()) cubeMap = Resources().Get<CubeMap>(textureName);

		textureName = "";
		SERIAL_READ_NAME(document, "shadowMap", textureName);
		if (!textureName.empty()) shadowMap = Resources().Get<Texture>(textureName);

		SERIAL_READ(document, baseColor);
		SERIAL_READ(document, emissiveColor);
		SERIAL_READ(document, shininess);
		SERIAL_READ(document, tiling);
		SERIAL_READ(document, offset);

		return true;
	}

	void Material::Bind() {
		parameters = Parameter::None;

		program->Use();
		if (baseMap) {
			baseMap->SetActive(GL_TEXTURE0);
			baseMap->Bind();
			program->SetUniform("u_baseMap", 0);
			parameters = (Parameter)((uint32_t)parameters | (uint32_t)Parameter::BaseMap);
		}

		if (specularMap) {
			specularMap->SetActive(GL_TEXTURE1);
			specularMap->Bind();
			program->SetUniform("u_specularMap", 1);
			parameters = (Parameter)((uint32_t)parameters | (uint32_t)Parameter::SpecularMap);
		}
		
		if (emissiveMap) {
			emissiveMap->SetActive(GL_TEXTURE2);
			emissiveMap->Bind();
			program->SetUniform("u_emissiveMap", 2);
			parameters = (Parameter)((uint32_t)parameters | (uint32_t)Parameter::EmissiveMap);
		}

		if (normalMap) {
			normalMap->SetActive(GL_TEXTURE3);
			normalMap->Bind();
			program->SetUniform("u_normalMap", 3);
			parameters = (Parameter)((uint32_t)parameters | (uint32_t)Parameter::NormalMap);
		}

		if (cubeMap) {
			cubeMap->SetActive(GL_TEXTURE4);
			cubeMap->Bind();
			program->SetUniform("u_cubeMap", 4);
			parameters = (Parameter)((uint32_t)parameters | (uint32_t)Parameter::CubeMap);
		}

		if (shadowMap) {
			shadowMap->SetActive(GL_TEXTURE5);
			shadowMap->Bind();
			program->SetUniform("u_shadowMap", 5);
			parameters = (Parameter)((uint32_t)parameters | (uint32_t)Parameter::ShadowMap);
		}

		program->SetUniform("u_material.baseColor", baseColor);
		program->SetUniform("u_material.emissiveColor", emissiveColor);
		program->SetUniform("u_material.shininess", shininess);
		program->SetUniform("u_material.tiling", tiling);
		program->SetUniform("u_material.offset", offset);
		program->SetUniform("u_material.parameters", (uint32_t)parameters);

		program->SetUniform("u_ior", ior);
	}

	void Material::UpdateGui() {

		ImGui::Separator();
		if (baseMap) {
			ImGui::Text("Base Map: %s", baseMap->name.c_str());
			Editor::ShowTexture(*baseMap, 32, 32);
			Editor::GetDialogResource<Texture>(baseMap, "BaseMapDialog", "Open texture", "Image (*.png;*.jpg;*.bmp;*.jpeg;*.tga;){.png,.jpg,.bmp,.jpeg,.tga},.*");
		}
		ImGui::ColorEdit3("Base Color", glm::value_ptr(baseColor));
		ImGui::Separator();

		ImGui::Separator();
		if (specularMap) {
			ImGui::Text("Specular Map: %s", specularMap->name.c_str());
			Editor::ShowTexture(*specularMap, 32, 32);
			Editor::GetDialogResource<Texture>(specularMap, "BaseMapDialog", "Open texture", "Image (*.png;*.jpg;*.bmp;*.jpeg;*.tga;){.png,.jpg,.bmp,.jpeg,.tga},.*");
		}
		ImGui::Separator();

		ImGui::Separator();
		if (emissiveMap) {
			ImGui::Text("Emmisive Map: %s", emissiveMap->name.c_str());
			Editor::ShowTexture(*emissiveMap, 32, 32);
			Editor::GetDialogResource<Texture>(emissiveMap, "BaseMapDialog", "Open texture", "Image (*.png;*.jpg;*.bmp;*.jpeg;*.tga;){.png,.jpg,.bmp,.jpeg,.tga},.*");
		}
		ImGui::ColorEdit3("Emmisive Color", glm::value_ptr(emissiveColor));
		ImGui::Separator();

		ImGui::Separator();
		if (normalMap) {
			ImGui::Text("Normal Map: %s", normalMap->name.c_str());
			Editor::ShowTexture(*normalMap, 32, 32);
			Editor::GetDialogResource<Texture>(normalMap, "BaseMapDialog", "Open texture", "Image (*.png;*.jpg;*.bmp;*.jpeg;*.tga;){.png,.jpg,.bmp,.jpeg,.tga},.*");
		}
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
			bool updated = false;
			updated |= ImGui::ColorEdit3("Base Color", glm::value_ptr(baseColor));
			updated |= ImGui::DragFloat("Shininess", &shininess, 1.0f);
			updated |= ImGui::DragFloat2("Tiling", glm::value_ptr(tiling), 0.1f);
			updated |= ImGui::DragFloat2("Offset", glm::value_ptr(offset), 0.1f);

			if (updated && program) {
				program->Use();
				program->SetUniform("u_material.baseColor", baseColor);
				program->SetUniform("u_material.shininess", shininess);
				program->SetUniform("u_material.tiling", tiling);
				program->SetUniform("u_material.offset", offset);
			}
		}
	}

}