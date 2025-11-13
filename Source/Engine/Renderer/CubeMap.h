#pragma once

namespace neu {
	class CubeMap : public Texture {
	public:
		//CLASS_PROTOTYPE(CubeMap);

		bool Load(const std::string& filenames);
	};
}