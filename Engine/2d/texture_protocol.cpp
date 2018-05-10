#include "common.h"
#include "texture_protocol.h"

namespace engine
{
    void texture_protocol::set_alpha(float alpha)
    {
        set_opacity(int(alpha * 255));
    }
    
    float texture_protocol::get_alpha() const
    {
        float alpha = get_opacity() / 255.0f;
        
        for (auto parent = get_parent(); parent != nullptr; parent = parent->get_parent())
            alpha *= parent->get_opacity() / 255.0f;
        
        return alpha;
    }
}
