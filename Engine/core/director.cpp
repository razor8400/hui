#include "common.h"

#include <chrono>

#include "director.h"
#include "application.h"

#include "platform/platform.h"

#include "renderer/renderer.h"
#include "resources/resources_manager.h"

#include "2d/label.h"

#include "scene.h"
#include "pool_manager.h"

namespace engine
{
    director& director::instance()
    {
        static director instance;
        return instance;
    }
    
    director::director()
    {
		m_renderer = std::make_unique<renderer>();
        
#if DRAW_STATS
        m_stats_label = label::create<label>("fonts/arial.ttf", 18);
		if (m_stats_label)
			m_stats_label->set_anchor(math::vector2d::zero);
#endif
    }
    
    director::~director()
    {

    }
    
    math::vector3d director::convert_screen_to_world(const math::vector2d& screen) const
    {
        auto win_size = application::instance().get_win_size();
        return screen - win_size / 2.0f;
    }
    
	void director::set_projection_mode(projection_mode mode)
	{
		m_renderer->set_projection_mode(mode);
	}

	void director::set_near_plane(float near_plane)
	{
		m_renderer->set_near_plane(near_plane);
	}

	void director::set_far_plane(float far_plane)
	{
		m_renderer->set_far_plane(far_plane);
	}

	void director::set_field_of_view(float field_of_view)
	{
		m_renderer->set_field_of_view(field_of_view);
	}

	void director::set_camera_position(const math::vector3d& position)
	{
		m_renderer->set_camera_position(position);
	}

	void director::start()
    {
		logger() << "[director] start";
		m_renderer->dump_camera_settings();
        m_running = true;
		m_reset_delta_time = true;
    }
    
    void director::stop()
    {
        m_running = false;
    }
    
    void director::run_scene(scene* scene)
    {
		logger() << "[director] run scene";
        
        m_next_scene = scene;
    }
    
    void director::main_loop()
    {
        if (m_paused)
            return;
        
        auto errors = gl::get_errors();
        
        for (auto error : errors)
            logger() << "[gl] " << error;
        
        gl::clear_errors();
        
        if (m_next_scene)
        {
            if (m_scene)
                m_scene->on_exit();
            
            m_scene = m_next_scene;
            m_scene->on_enter();
            
            m_next_scene = nullptr;
        }
        
        if (m_running)
        {
            auto delta = get_delta_time();
            
            update(delta);
        }
    }
    
    void director::update(float delta_time)
    {
        if (m_scene)
        {
            m_scene->update(delta_time);
			m_renderer->draw_scene(m_scene);
        }
        
        ++m_frames;
        m_time += delta_time;
        
        pool_manager::instance().update();
        
#if DRAW_STATS
        draw_stats();
#endif
    }
    
#if DRAW_STATS
    void director::draw_stats()
    {
        auto win_size = application::instance().get_win_size();
     
        std::stringstream stats;
        
        stats.precision(3);
		stats << "fps:" << calculate_fps() << "\n"
			<< "draw calls:" << gl::get_draw_calls() / m_frames << " per frame" << "\n"
			<< "memory used:" << platform::instance().get_memory_used() << "KB";
    
		if (m_stats_label)
		{
			m_stats_label->set_caption(stats.str());
			m_stats_label->set_position(math::vector2d(-win_size.x / 2, win_size.y / 2 - m_stats_label->get_size().y));

			m_stats_label->update(0);
			m_stats_label->draw(m_renderer->get_world());
		}
    }
#endif
    
    float director::get_delta_time() const
    {
        static auto last_update = std::chrono::steady_clock::now();
        
        auto now = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_update);
        
        last_update = now;
        
        if (m_reset_delta_time)
        {
            m_reset_delta_time = false;
            return 0;
        }
        
        return delta.count();
    }
    
    time_t director::get_local_time() const
    {
        auto now = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

        return ms.count();
    }
    
    const math::mat4& director::get_world() const
    {
        return m_renderer->get_world();
    }
    
    float director::calculate_fps() const
    {       
        return m_frames / m_time;
    }
    
    void director::on_focus()
    {
        m_paused = false;
        m_reset_delta_time = true;
    }
    
    void director::on_lost_focus()
    {
        m_paused = true;
    }
}
