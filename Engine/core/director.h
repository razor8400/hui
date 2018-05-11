#pragma once

#include "common.h"

namespace engine
{
	class renderer;
    
    class director
    {
    public:
        static director& instance();
        
        math::vector3d convert_screen_to_world(const math::vector2d& screen) const;
        
		void set_projection_mode(projection_mode mode);
		
		void set_near_plane(float near_plane);
		void set_far_plane(float far_plane);
		void set_field_of_view(float field_of_view);

		void set_camera_position(const math::vector3d& position);
        
        void on_focus();
        void on_lost_focus();

		void start();
        void stop();
				
        void run_scene(scene* scene);
        
		void main_loop();
        
        scene* running_scene() const { return m_scene; }
        float get_delta_time() const;
        time_t get_local_time() const;
        
        const math::mat4& get_world() const;
        
        float calculate_fps() const;
	private:
        void update(float delta_time);
        
#if DRAW_STATS
        void draw_stats();
#endif
        
        director();
        ~director();
    private:
        pointer<scene> m_scene;
        pointer<scene> m_next_scene;
        
#if DRAW_STATS
        pointer<label> m_stats_label;
#endif
        
        int m_frames = 0;
        float m_time = 0;

		bool m_running = false;
        bool m_paused = false;
        mutable bool m_reset_delta_time = false;

		std::unique_ptr<renderer> m_renderer;
    };
}
