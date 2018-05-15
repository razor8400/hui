#include "common.h"

#include "utils/file_utils.h"
#include "texture2d.h"
#include "font_ttf.h"

namespace engine
{
    static const int max_atlas = 1024;
    
    font_ttf::font_ttf(const std::string& font_name) : m_font_name(font_name)
    {
        
    }
    
    font_ttf::~font_ttf()
    {
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
    
    texture2d_ptr font_ttf::create_label(const std::string& text, int size, int max_line_width,
                                         vertical_text_alignment vertical_alignment,
                                         horisontal_text_alignment horisontal_alignment,
                                         std::vector<gl::v3f_c4f_t2f>* vertices)
    {
        if (text.empty())
            return texture2d_ptr();
        
        auto& atlas = get_atlas(text, size);
        
        auto width = atlas.texture->get_width();
        auto height = atlas.texture->get_height();
        
        int x = 0;
        int y = height;
        
        for (auto& ch : text)
        {
			if (ch == '\n' || (max_line_width > 0 && x >= max_line_width))
			{
				y -= size;
				x = 0;
				continue;
			}

            auto it = atlas.glyphs.find(ch);
            
            if (it == atlas.glyphs.end())
                continue;
            
            auto& glyph = it->second;
            
            float tx = glyph.tx;
            float ty = glyph.ty;
            
            auto quad = gl::v3f_c4f_t2f_quad();
            
            quad[gl::bottom_left].vertice = { x + glyph.bl, y + glyph.bt, 0 };
            quad[gl::bottom_right].vertice = { x + glyph.bl + glyph.bw, y + glyph.bt, 0 };
            quad[gl::top_right].vertice = { x + glyph.bl + glyph.bw, y + glyph.bt - glyph.bh, 0 };
            quad[gl::top_left].vertice = { x + glyph.bl, y + glyph.bt - glyph.bh, 0 };
            
            quad[gl::bottom_left].tex_coord = { tx, ty };
            quad[gl::bottom_right].tex_coord = { tx + (float)glyph.bw / width, ty };
            quad[gl::top_right].tex_coord = { tx + (float)glyph.bw / width, ty + ((float)glyph.bh / height) };
            quad[gl::top_left].tex_coord = { tx, ty + ((float)glyph.bh / height) };
            
            vertices->insert(vertices->end(), quad.begin(), quad.end());
            
            x += glyph.ax;
        }
        
        return atlas.texture;
    }
    
    const font_ttf::atlas& font_ttf::get_atlas(const std::string& text, int size)
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
        
        auto info = font_utils::create_atlas(m_font_name, size, buffer, max_atlas);
        auto texture = std::make_shared<texture2d>(info.width, info.height, GL_RED, info.texture);
        
        m_cache[size] = { texture, info.glyphs };
    }
    
    math::vector2d font_ttf::text_size(const std::string& text, int size, int max_line_width) const
    {
        auto sz = font_utils::text_size(m_font_name, size, text, max_line_width);
        return math::vector2d(sz.w, sz.h);
    }
}
