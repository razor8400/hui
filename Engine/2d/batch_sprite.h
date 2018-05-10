#pragma once

#include "2d/sprite.h"

namespace engine
{   
    class batch_sprite : public sprite
    {
		DECLARE_CLASS;
    public:       
        void add_child(game_object* obj) override;
        void remove_child(game_object* obj) override;
        
        void draw(const math::mat4& parent) override;
    private:
        vector<sprite*> m_sprites;
    };
}
