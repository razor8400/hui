#pragma once

namespace engine
{
	class renderer
	{
	public:
		void set_projection_mode(projection_mode mode);

		void set_near_plane(float near_plane);
		void set_far_plane(float far_plane);
		void set_field_of_view(float field_of_view);

		void set_camera_position(const math::vector3d& position);
		void draw_scene(scene* scene);
        void add_command(const render_command_ptr& command);
        void add_post_draw_command(const render_command_ptr& command);
		void dump_camera_settings();
        
        const math::mat4& get_world() const { return m_world; }
	private:
		void update_world();
	private:
		float m_near_plane = 0;
		float m_far_plane = 0;
		float m_field_of_view = 0;

		projection_mode m_projection_mode;

		math::vector3d m_camera_position = math::vector3d(0, 0, -1);
		math::mat4 m_world;
        
        std::vector<render_command_ptr> m_draw_commands;
        std::vector<render_command_ptr> m_post_draw_commands;
	};
}
