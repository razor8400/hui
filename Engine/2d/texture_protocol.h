#pragma once

#include "texture_protocol.h"

namespace engine
{
    class texture_protocol : public game_object
    {
    public:
        void set_alpha(float alpha);
        float get_alpha() const;
        
        void set_color(const math::vector3d& color) { m_color = color; }
        const math::vector3d& get_color() const { return m_color; }
        
        math::vector4d get_color_rgba() const { return math::vector4d(m_color.x, m_color.y, m_color.z, get_alpha()); }
        
        void set_blend_func(const gl::blend_func& blend_func) { m_blend_func = blend_func; }
        const gl::blend_func& get_blend_func() const { return m_blend_func; }
    protected:
        math::vector3d m_color = math::vector3d::one;
        gl::blend_func m_blend_func = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
    };
}
