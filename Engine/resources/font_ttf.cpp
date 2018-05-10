#include "common.h"
#include "utils/file_utils.h"
#include "font_ttf.h"

namespace engine
{
    font_ttf::font_ttf(const std::string& font_name) : m_font_name(font_name)
    {
        
    }
    
    font_ttf::~font_ttf()
    {
        for (auto& it : m_cache)
            gl::delete_texture(it.second.texture);
        font_utils::unload_font(m_font_name);
    }
    
	std::shared_ptr<font_ttf> font_ttf::load_from_file(const std::string& file_name)
	{
		logger() << "[font_ttf] load:" << file_name;

        auto font_name = file_utils::get_file_name(file_name);
        
        if (font_utils::load_font(file_name, font_name))
            return std::make_shared<font_ttf>(font_name);
        
		return std::shared_ptr<font_ttf>();
	}
    
    void font_ttf::render_text(const std::string& text, int size, const math::vector4d& color)
    {
        if (text.empty())
            return;
        
        auto& atlas = get_atlas(text, size);
        
        gl::bind_texture(atlas.texture);
        
        int x = 0;
        int y = 0;
        
        std::vector<math::vector2d> vertices;
        std::vector<math::vector2d> tex_coords;
        std::vector<math::vector4d> colors;
        
        static const int quad = 4;
        
        for (auto& ch : text)
        {
            auto it = atlas.glyphs.find(ch);
            
            if (it == atlas.glyphs.end())
                continue;
            
            auto& glyph = it->second;
            
            vertices.push_back({ x + glyph.bl, y + glyph.bt });
            vertices.push_back({ x + glyph.bl + glyph.bw, y + glyph.bt });
            vertices.push_back({ x + glyph.bl + glyph.bw, y + glyph.bt - glyph.bh });
            vertices.push_back({ x + glyph.bl, y + glyph.bt - glyph.bh });
            
            float tx = glyph.tx;
            float ty = glyph.ty;
            
            tex_coords.push_back({ tx, ty });
            tex_coords.push_back({ tx + (float)glyph.bw / atlas.width, ty });
            tex_coords.push_back({ tx + (float)glyph.bw / atlas.width, ty + ((float)glyph.bh / atlas.height) });
            tex_coords.push_back({ tx, ty + ((float)glyph.bh / atlas.height) });
            
            for (int i = 0; i < quad; ++i)
                colors.push_back(color);
            
            x += glyph.ax;
        }
        
        gl::draw_texture2d(vertices, tex_coords, colors);
    }
    
    const font_utils::atlas& font_ttf::get_atlas(const std::string& text, int size)
    {
        auto it = m_cache.find(size);
        
        if (it != m_cache.end())
        {
            auto& glyphs = it->second.glyphs;
            std::string buffer;
            for (auto& ch : text)
            {
                if (glyphs.find(ch) != glyphs.end())
                    continue;
                
                buffer.push_back(ch);
            }
            
            if (buffer.size() > 0)
                update_atlas(buffer, size);
        }
        else
        {
            update_atlas(text, size);
        }
        
        return m_cache[size];
    }
    
    void font_ttf::update_atlas(const std::string& text, int size)
    {
        auto& atlas = m_cache[size];
        std::string buffer;
        
        for (auto& it : atlas.glyphs)
            buffer.push_back(it.first);
        
        buffer.insert(buffer.end(), text.begin(), text.end());
        
        m_cache[size] = font_utils::create_atlas(m_font_name, size, buffer);
    }
    
    math::vector2d font_ttf::text_size(const std::string& text, int size) const
    {
        auto sz = font_utils::text_size(m_font_name, size, text);
        return math::vector2d(sz.w, sz.h);
    }
}
