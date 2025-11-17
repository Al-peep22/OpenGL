#pragma once

namespace neu {
	class Editor{
	public:
		void Begin();
		void UpdateGui(class Scene& scene);

		void ShowTexture(const Texture& texture, float width, float height);

	private:
		bool m_active = true;
		class Object* m_selected = nullptr;
	};
}