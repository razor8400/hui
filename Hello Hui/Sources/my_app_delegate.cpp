#include "engine.h"
#include "my_app_delegate.h"

void my_app_delegate::application_launched(engine::application* application)
{
	application->set_win_size(1024, 768);
	application->set_display_name("Hello Hui");
 
    if (application->create_context_window())
    {
        engine::director& director = engine::director::instance();
        
        director.set_projection_mode(engine::ortographic);
		director.set_near_plane(-1000.0f);
        director.set_far_plane(1000.0f);
        
        director.start();
        
        run_match3_scene();
        
        application->run();
    }
}

void my_app_delegate::application_terminated(engine::application* application)
{
    application->shutdown();
}

void my_app_delegate::application_enter_background(engine::application* application)
{
    engine::resources_manager::instance().clear_cache();
}

void my_app_delegate::application_enter_foreground(engine::application* application)
{
    engine::logger() << "[my_app_delegate] restart current scene";
    run_match3_scene();
}

void my_app_delegate::run_match3_scene()
{
    engine::director& director = engine::director::instance();
    
    auto scene = engine::game_object::create<engine::scene>();
    auto label = engine::game_object::create<engine::label>("fonts/arial.ttf", 50);
    
    label->set_caption("Hello Hui");
    label->set_position(math::vector2d(-100, 0));
    scene->add_child(label);
    
    director.run_scene(scene);
}

