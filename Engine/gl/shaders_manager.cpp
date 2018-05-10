#include "shaders.h"
#include "shader_program.h"
#include "shaders_manager.h"

namespace gl
{
    shaders_manager& shaders_manager::instance()
    {
        static shaders_manager instance;

        return instance;
    }

    template<class T>
    shader_program_ptr shaders_manager::create_gl_program(const char* vert, const char* frag)
    {
        auto program = gl::create_gl_program(vert, frag);

        return std::make_shared<T>(program);
    }

    shaders_manager::shaders_manager()
    {

    }

    void shaders_manager::compile_default_shaders()
    {
        auto position_color = create_gl_program<shader_position_color>(shaders::shader_position_color_vert, shaders::shader_position_color_frag);
        auto shader_position_texture = create_gl_program<shader_texture_position_color>(shaders::shader_texture_position_vert, shaders::shader_texture_position_frag);
        auto shader_position_texture_color = create_gl_program<shader_texture_position_color>(shaders::shader_texture_position_color_vert, shaders::shader_texture_position_color_frag);
        auto shader_position_texture_color_alpha = create_gl_program<shader_texture_position_color>(shaders::shader_texture_position_color_alpha_vert, shaders::shader_texture_position_color_alpha_frag);
		auto shader_position_font_color_alpha = create_gl_program<shader_texture_position_color>(shaders::shader_texture_position_color_alpha_vert, shaders::shader_font_position_color_alpha_frag);

        add_to_cache(position_color, shader_program::shader_position_color);
        add_to_cache(shader_position_texture, shader_program::shader_texture_position);
        add_to_cache(shader_position_texture_color, shader_program::shader_texture_position_color);
        add_to_cache(shader_position_texture_color_alpha, shader_program::shader_texture_position_color_alpha);
		add_to_cache(shader_position_font_color_alpha, shader_program::shader_font_position_color_alpha);
    }

    void shaders_manager::add_to_cache(const shader_program_ptr& program, const std::string& key)
    {
        m_cache[key] = program;
    }

    const shader_program_ptr& shaders_manager::get_program(const std::string& key) const
    {
        auto it = m_cache.find(key);

        if (it != m_cache.end())
            return it->second;

        static shader_program_ptr null_program;

        return null_program;
    }
}
