#include "common.h"
#include "renderer.h"
#include "render_command.h"

#include "core/application.h"
#include "core/scene.h"

namespace engine
{
	void renderer::set_projection_mode(projection_mode mode)
	{
		m_projection_mode = mode;
		update_world();
	}

	void renderer::set_near_plane(float near_plane)
	{
		m_near_plane = near_plane;
		update_world();
	}

	void renderer::set_far_plane(float far_plane)
	{
		m_far_plane = far_plane;
		update_world();
	}

	void renderer::set_field_of_view(float field_of_view)
	{
		m_field_of_view = field_of_view;
		update_world();
	}

	void renderer::set_camera_position(const math::vector3d& position)
	{
		m_camera_position = position;
		update_world();
	}

	void renderer::draw_scene(scene* scene)
	{
        gl::clear();
        
		if (scene)
			scene->draw(this);
        
        for (auto& command : m_draw_commands)
        {
			command->execute(m_world);
			command->reset();
        }
        
        for (auto& command : m_post_draw_commands)
        {
			command->execute(m_world);
			command->reset();
        }
        
        m_draw_commands.clear();
        m_post_draw_commands.clear();
	}
    
    void renderer::add_command(const render_command_ptr& command)
    {
        m_draw_commands.push_back(command);
    }

    void renderer::add_post_draw_command(const render_command_ptr& command)
    {
        m_post_draw_commands.push_back(command);
    }
    
	void renderer::dump_camera_settings()
	{
		logger() << "[camera] "
			<< "mode:" << projection_mode_to_string(m_projection_mode) << ", "
			<< "position:" << vector3d_to_string(m_camera_position) << ", "
			<< "field of view:" << m_field_of_view << ", "
			<< "far plane:" << m_far_plane << ", "
			<< "near plane:" << m_near_plane;
	}

	void renderer::update_world()
	{
		auto win_size = application::instance().get_win_size();

		auto camera = math::mat4::look_at(m_camera_position, math::vector3d::zero, math::vector3d::up);

		if (m_projection_mode == perspective)
		{
			m_world = math::mat4::perspective(m_field_of_view, win_size.x / win_size.y, m_near_plane, m_far_plane) * camera;
		}
		else if (m_projection_mode == ortographic)
		{
			m_world = math::mat4::ortographic(win_size.x, win_size.y, m_near_plane, m_far_plane) * camera;
		}
	}
}
