#include "common.h"
#include "sprite.h"
#include "batch_sprite.h"

namespace engine
{   
	IMPLEMENT_INHERITANCE_INFO(batch_sprite, sprite);

    void batch_sprite::add_child(game_object* obj)
    {
        auto sprite = dynamic_cast<class sprite*>(obj);
        
        assert(sprite && sprite->get_texture() == get_texture());
        
        m_sprites.push_back(sprite);
        
        game_object::add_child(obj);
    }
    
    void batch_sprite::remove_child(game_object* obj)
    {
        auto sprite = dynamic_cast<class sprite*>(obj);
        
        assert(sprite);
        
        m_sprites.erase(sprite);
        
        game_object::remove_child(obj);
    }   
    
    void batch_sprite::draw(const math::mat4& t)
    {
        if (!m_enabled)
            return;
        
        auto model_view_transform = transform(t);

		m_quad.vertices.clear();
		m_quad.colors.clear();
		m_quad.uv.clear();
        
        for (auto& obj : m_sprites)
        {
			if (!obj->get_enabled())
				continue;

			auto transform = obj->get_transform();
			auto quad = obj->update_quad();

			for (auto& vertice : quad.vertices)
				vertice = math::transform_point(vertice, transform);

			m_quad.vertices.insert(m_quad.vertices.end(), quad.vertices.begin(), quad.vertices.end());
			m_quad.uv.insert(m_quad.uv.end(), quad.uv.begin(), quad.uv.end());
			m_quad.colors.insert(m_quad.colors.end(), quad.colors.begin(), quad.colors.end());
        }
        
        render(model_view_transform);
    }
}
