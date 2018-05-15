#pragma once

#include "resource.h"
#include "utils/font_utils.h"

namespace engine
{
	class font_ttf : public resource
	{
        struct atlas
        {
            texture2d_ptr texture;
            font_utils::glyphs_map glyphs;
        };
	public:
        font_ttf(const std::string& font_name);
        ~font_ttf();
        
		static std::shared_ptr<font_ttf> load_from_file(const std::string& file_name);
        bool load(const unsigned char* data, size_t size) override { return true; }
        
        texture2d_ptr create_label(const std::string& text, int size, int max_line_width,
                                    vertical_text_alignment vertical_alignment,
                                    horisontal_text_alignment horisontal_alignment,
                                    std::vector<gl::v3f_c4f_t2f>* vertices);
        math::vector2d text_size(const std::string& text, int size, int max_line_width) const;
    private:
        void update_atlas(const std::string& text, int size);
        const atlas& get_atlas(const std::string& text, int size);
	private:
        std::string m_font_name;
		std::map<int, atlas> m_cache;
	};
}
