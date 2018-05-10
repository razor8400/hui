#include "common.h"
#include "resources/texture2d.h"
#include "resources/texture_atlas.h"
#include "resources/resources_manager.h"
#include "sprite.h"

namespace engine
{
    IMPLEMENT_INHERITANCE_INFO(sprite, game_object);
    
    bool sprite::init()
    {
        if (!game_object::init())
            return false;

        m_shader_program = gl::shaders_manager::instance().get_program(gl::shader_program::shader_texture_position_color_alpha);
        
        return true;
    }

    bool sprite::init(const std::string& file_name)
    {
        auto texture = resources_manager::instance().load_resource_from_file<texture2d>(file_name);

        return init(texture);
    }
    
    bool sprite::init(const texture2d_ptr& texture)
    {
        if (!texture || !init())
            return false;

		set_texture(texture);
                    
        return true;
    }

	bool sprite::init(const texture2d_ptr& texture, const math::rect& rect)
	{
		if (!texture || !init())
			return false;

		set_texture(texture, rect);

		return true;
	}

	bool sprite::init(const std::string& atlas_name, const std::string& file_name)
	{
		auto atlas = resources_manager::instance().load_resource_from_file<texture_atlas>(atlas_name);

		if (!atlas)
			return false;

		sprite_frame frame;

		if (!atlas->get_frame(file_name, &frame))
			return false;

		m_rotated = frame.rotated;
		m_size = frame.source_size;

		return init(atlas->get_texture(), frame.frame);
	}
    
	void sprite::draw(const math::mat4& t)
	{
		if (m_texture)
			m_quad = update_quad();

		game_object::draw(t);
	}

	void sprite::render(const math::mat4& t)
    {
		auto texture = get_texture();

        if (texture)
        {
			if (m_shader_program)
				m_shader_program->use(t);

            gl::bind_texture(texture->get_texture_id());
            gl::set_blend_func(m_blend_func.source, m_blend_func.destination);
            gl::draw_texture2d(m_quad.vertices, m_quad.uv, m_quad.colors);
        }
        
		game_object::render(t);
    }

	void sprite::clear_texture()
	{
        m_texture.reset();
	}

	void sprite::set_texture(const std::string& file_name)
    {
        auto texture = resources_manager::instance().load_resource_from_file<texture2d>(file_name);

        set_texture(texture);
    }
    
    void sprite::set_texture(const texture2d_ptr& texture)
    {
        clear_texture();
        
        if (texture)
        {
            auto size = math::vector2d(texture->get_width(), texture->get_height());
            
            set_texture(texture, math::rect(0, 0, size.x, size.y));
            set_size(size);
        }
    }
    
    void sprite::set_texture(const texture2d_ptr& texture, const math::rect& rect)
    {
        m_texture = texture;
		m_texture_rect = rect;
    }

    sprite::quad sprite::update_quad() const
	{
        assert(m_texture);
        
		auto origin = m_texture_rect.get_origin();
        auto frame_size = m_texture_rect.get_size();
        
		auto texture_size = math::vector2d(m_texture->get_width(), m_texture->get_height());
		auto offset = (math::vector2d(m_size.x, m_size.y) - frame_size) / 2;

		auto color = get_color_rgba();

		quad quad;

		quad.vertices.push_back(offset);
        quad.vertices.push_back({ offset.x + frame_size.x, offset.y });
        quad.vertices.push_back({ frame_size + offset });
        quad.vertices.push_back({ offset.x, frame_size.y + offset.y });

		if (m_rotated)
		{
            quad.uv.push_back({ origin.x / texture_size.x, origin.y / texture_size.y });
            quad.uv.push_back({ (origin.x + frame_size.y) / texture_size.x, origin.y / texture_size.y });
            quad.uv.push_back({ (origin.x + frame_size.y) / texture_size.x, (origin.y + frame_size.x) / texture_size.y });
            quad.uv.push_back({ origin.x / texture_size.x, (origin.y + frame_size.x) / texture_size.y });

			for (auto& vertice : quad.vertices)
			{
				auto t = vertice.x;
				vertice.x = vertice.y;
				vertice.y = t;
			}
		}
		else
		{
            quad.uv.push_back({ origin.x / texture_size.x, (origin.y + frame_size.y) / texture_size.y });
            quad.uv.push_back({ (origin.x + frame_size.x) / texture_size.x, (origin.y + frame_size.y) / texture_size.y });
            quad.uv.push_back({ (origin.x + frame_size.x) / texture_size.x, origin.y / texture_size.y });
            quad.uv.push_back({ origin.x / texture_size.x, origin.y / texture_size.y });
		}
        
        for (auto i = 0; i < quad.vertices.size(); ++i)
            quad.colors.push_back(color);

		return quad;
	}
}
