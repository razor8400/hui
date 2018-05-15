#include "common.h"
#include "font_utils.h"

#include "ft2build.h"
#include FT_FREETYPE_H 

namespace engine
{
    class free_type_library
    {
    public:
        free_type_library();
        ~free_type_library();
        
		bool library_loaded() const;
        bool load_font(const std::string& file_name, const std::string& font_name) const;
		bool load_glyphs(font_utils::atlas_info* atlas, const std::string& font_name, int font_size, const std::string& text) const;
        
        font_utils::size text_size(const std::string& font_name, int font_size, const std::string& text, int max_width) const;
        void unload_font(const std::string& font_name);
    private:
        FT_Library m_libary;
        bool m_library_loaded = true;
        mutable std::map<std::string, FT_Face> m_loaded_fonts;
    };
    
    free_type_library::free_type_library()
    {
        logger() << "[free_type] load library";
        
        if (FT_Init_FreeType(&m_libary))
        {
            logger() << "[free_type] could not init FreeType library";
            m_library_loaded = false;
        }
    }
    
    free_type_library::~free_type_library()
    {
        logger() << "[free_type] unload library";
        
        if (m_library_loaded)
        {
            for (auto it : m_loaded_fonts)
                FT_Done_Face(it.second);
            
            FT_Done_FreeType(m_libary);
        }
    }

	bool free_type_library::library_loaded() const
	{
		if (!m_library_loaded)
		{
			logger() << "[free type] library not loaded";
			return false;
		}

		return true;
	}
    
    bool free_type_library::load_font(const std::string& file_name, const std::string& font_name) const
    {
		if (!library_loaded())
			return false;
        
        auto it = m_loaded_fonts.find(font_name);
        
        if (it == m_loaded_fonts.end())
        {
            FT_Face face;
            
            logger() << "[free type] load font:" << font_name;
            
            if (FT_New_Face(m_libary, file_name.c_str(), 0, &face))
                return false;
            
            m_loaded_fonts[font_name] = face;
        }
        
        return true;
    }

	bool free_type_library::load_glyphs(font_utils::atlas_info* atlas, const std::string& font_name, int font_size, const std::string& text) const
	{
        assert(atlas);

		if (!library_loaded())
			return false;

		auto it = m_loaded_fonts.find(font_name);

		if (it == m_loaded_fonts.end())
		{
			logger() << "[free type] font not loaded:" << font_name;
			return false;
		}

		auto face = it->second;
		FT_Set_Pixel_Sizes(face, 0, font_size);

		int x = 0;
        int y = 0;
        
        auto& map = atlas->glyphs;

		for (auto& ch : text)
		{
			if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
			{
				logger() << "[free type] failed to load glyph:" << ch;
				continue;
			}

			auto glyph = font_utils::glyph();
			auto ft = face->glyph;

			glyph.ax = ft->advance.x >> 6;
			glyph.ay = ft->advance.y >> 6;
			glyph.bw = ft->bitmap.width; 
			glyph.bh = ft->bitmap.rows;
			glyph.bl = ft->bitmap_left;
			glyph.bt = ft->bitmap_top;
            glyph.tx = (float)x / atlas->width;
            glyph.ty = (float)y / atlas->height;

			gl::sub_image2d(atlas->texture, GL_TEXTURE_2D, 0, x, y, glyph.bw, glyph.bh, GL_RED, GL_UNSIGNED_BYTE, ft->bitmap.buffer);

			x += ft->bitmap.width;

			map[ch] = glyph;
		}

		return true;
	}
        
	font_utils::size free_type_library::text_size(const std::string& font_name, int font_size, const std::string& text, int max_width) const
	{
		if (!library_loaded())
			return font_utils::size();

		auto it = m_loaded_fonts.find(font_name);

		if (it == m_loaded_fonts.end())
		{
			logger() << "[free type] font not loaded:" << font_name;
			return font_utils::size();
		}

		auto face = it->second;

		FT_Set_Pixel_Sizes(face, 0, font_size);
		
		int w = 0;
        
        font_utils::size size;

		size.h = font_size;

		for (auto& ch : text)
		{
			if (ch == '\n' || (max_width > 0 && w >= max_width))
			{
				size.w = w;
				size.h += font_size;
				w = 0;
			}

			if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
			{
				logger() << "[free type] failed to load glyph:" << ch;
				continue;
			}

			auto glyph = face->glyph;

			w += glyph->advance.x >> 6;
		}
        
        size.w = std::max(size.w, w);
		
        return size;
	}

	void free_type_library::unload_font(const std::string& font_name)
    {
        logger() << "[free type] unload font:" << font_name;
        
		if (!library_loaded())
			return;
        
        auto it = m_loaded_fonts.find(font_name);
        
        if (it != m_loaded_fonts.end())
        {
			FT_Done_Face(it->second);
			m_loaded_fonts.erase(it);
        }      
    }
    
    namespace font_utils
    {
        static free_type_library library;

		bool load_font(const std::string& file_name, const std::string& font_name)
        {
            if (!library.load_font(file_name, font_name))
            {
                logger() << "[font utils] error loading font:" << font_name;
                return false;
            }
            
            return true;
        }
                        
        void unload_font(const std::string& font_name)
        {
            library.unload_font(font_name);
        }
		
		atlas_info create_atlas(const std::string& font_name, int font_size, const std::string& text, int max_width)
		{
			auto at = atlas_info();
			auto size = text_size(font_name, font_size, text, max_width);

			at.texture = gl::load_texture(0, size.w, size.h, GL_RED);
            at.width = size.w;
            at.height = size.h;
            
            if (!library.load_glyphs(&at, font_name, font_size, text))
                logger() << "[font utils] error loading atlas:" << text;

			return at;
		}

		size text_size(const std::string& font_name, int font_size, const std::string& text, int max_width)
		{
			return library.text_size(font_name, font_size, text, max_width);
		}
    }
}
