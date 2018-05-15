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
        set_texture_rect(rect);
    }
    
    void sprite::update_vertices()
    {
        if (!m_texture)
            return;
        
        auto origin = m_texture_rect.get_origin();
        auto frame_size = m_texture_rect.get_size();
        
        auto texture_size = math::vector2d(m_texture->get_width(), m_texture->get_height());
        auto offset = (m_size - frame_size) / 2;
        
        m_vertices.resize(gl::quad_size);
        
        m_vertices[gl::bottom_left].vertice = offset;
        m_vertices[gl::bottom_right].vertice = { offset.x + frame_size.x, offset.y, 0.0f };
        m_vertices[gl::top_right].vertice = { math::vector3d(frame_size.x, frame_size.y, 0.0f) + offset };
        m_vertices[gl::top_left].vertice = { offset.x, frame_size.y + offset.y, 0.0f };
        
        if (m_rotated)
        {
            m_vertices[gl::bottom_left].tex_coord = { origin.x / texture_size.x, origin.y / texture_size.y };
            m_vertices[gl::bottom_right].tex_coord = { (origin.x + frame_size.y) / texture_size.x, origin.y / texture_size.y };
            m_vertices[gl::top_right].tex_coord = { (origin.x + frame_size.y) / texture_size.x, (origin.y + frame_size.x) / texture_size.y };
            m_vertices[gl::top_left].tex_coord = { origin.x / texture_size.x, (origin.y + frame_size.x) / texture_size.y };
            
            for (auto& v : m_vertices)
            {
                auto t = v.vertice.x;
                v.vertice.x = v.vertice.y;
                v.vertice.y = t;
            }
        }
        else
        {
            m_vertices[gl::bottom_left].tex_coord = { origin.x / texture_size.x, (origin.y + frame_size.y) / texture_size.y };
            m_vertices[gl::bottom_right].tex_coord = { (origin.x + frame_size.x) / texture_size.x, (origin.y + frame_size.y) / texture_size.y };
            m_vertices[gl::top_right].tex_coord = { (origin.x + frame_size.x) / texture_size.x, origin.y / texture_size.y };
            m_vertices[gl::top_left].tex_coord = { origin.x / texture_size.x, origin.y / texture_size.y };
        }
    }
}
