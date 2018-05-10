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
        
        void render(const math::mat4& t) override;
        
        const font_ttf_ptr& get_font() const { return m_font; }
        void set_font(const font_ttf_ptr& font) { m_font = font; update_size(); }
        
        const std::string& get_caption() const { return m_caption; }
        void set_caption(const std::string& caption) { m_caption = caption; update_size(); }

		int get_font_size() const { return m_font_size; }
        void set_font_size(int font_size) { m_font_size = font_size; update_size(); }
    private:
        void update_size();
    private:
        std::string m_caption;
        int m_font_size;
        font_ttf_ptr m_font;
    };
}
