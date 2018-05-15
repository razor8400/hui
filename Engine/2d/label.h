#pragma once

#include "core/game_object.h"

namespace engine
{
    class label : public texture_protocol
    {
    public:
        bool init(const std::string& font_name);
        bool init(const std::string& font_name, int font_size);
        bool init(const std::string& font_name, int font_size, const std::string& caption);
        
        void set_font(const std::string& font_name);
        
        const font_ttf_ptr& get_font() const { return m_font; }
        void set_font(const font_ttf_ptr& font) { m_font = font; update_texture(); update_size(); }
        
        const std::string& get_caption() const { return m_caption; }
        void set_caption(const std::string& caption) { m_caption = caption; update_texture(); update_size(); }

		int get_font_size() const { return m_font_size; }
        void set_font_size(int font_size) { m_font_size = font_size; update_texture(); update_size(); }

		int get_max_line_width() const { return m_max_line_width; }
        void set_max_line_width(int max_line_width) { m_max_line_width = max_line_width; update_texture(); }

		vertical_text_alignment get_vertical_alignment() const { return m_vertical_alignment; }
        void set_vertical_alignment(vertical_text_alignment vertical_alignment) { m_vertical_alignment = vertical_alignment; update_texture(); }

		horisontal_text_alignment get_horisonal_alignment() const { return m_horisontal_alignment; }
        void get_horisonal_alignment(horisontal_text_alignment horisontal_alignment) { m_horisontal_alignment = horisontal_alignment; update_texture(); }
    private:
        void update_size();
        void update_texture();
    private:
        std::string m_caption;

        int m_font_size;
		int m_max_line_width;
        
		vertical_text_alignment m_vertical_alignment = vertical_text_alignment::left;
		horisontal_text_alignment m_horisontal_alignment = horisontal_text_alignment::bottom;

		font_ttf_ptr m_font;
    };
}
