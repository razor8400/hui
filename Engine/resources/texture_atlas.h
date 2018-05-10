#pragma once

#include "resource.h"

namespace engine
{
	struct sprite_frame
	{
		math::rect frame;
		math::vector2d source_size;
		bool rotated;
	};

	class texture_atlas : public resource
	{
	public:
		static std::shared_ptr<texture_atlas> load_from_file(const std::string& file_name);

		bool load(const unsigned char* data, size_t size) override;
		const texture2d_ptr& get_texture() const { return m_texture; }
		bool get_frame(const std::string& file_name, sprite_frame* frame) const;
	private:
		texture2d_ptr m_texture;
		std::map<std::string, sprite_frame> m_frames;
	};
}
