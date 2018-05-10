#include "common.h"
#include "resources/resources_manager.h"
#include "resources/font_ttf.h"
#include "label.h"

namespace engine
{   
    bool label::init(const std::string& font_name)
    {
        return init(font_name, 0);
    }
    
    bool label::init(const std::string& font_name, int font_size)
    {
        return init(font_name, font_size, empty_string);
    }
    
    bool label::init(const std::string& font_name, int font_size, const std::string& caption)
    {
        if (!game_object::init())
            return false;
        
        auto font = resources_manager::instance().load_resource_from_file<font_ttf>(font_name);
        
        if (!font)
            return false;
        
        m_font_size = font_size;
        m_caption = caption;
		m_shader_program = gl::shaders_manager::instance().get_program(gl::shader_program::shader_font_position_color_alpha);

        set_font(font);

        return true;
    }
    
    void label::set_font(const std::string& font_name)
    {
        auto font = resources_manager::instance().load_resource_from_file<font_ttf>(font_name);
        set_font(font);
    }
    
	void label::render(const math::mat4& t)
	{      
		if (m_font)
		{
            if (m_shader_program)
                m_shader_program->use(t);
            
            gl::set_blend_func(m_blend_func.source, m_blend_func.destination);
            
            m_font->render_text(m_caption, m_font_size, get_color_rgba());
		}
        
        game_object::render(t);
	}
    
    void label::update_size()
    {
        if (m_font)
            m_size = m_font->text_size(m_caption, m_font_size);
    }
}
