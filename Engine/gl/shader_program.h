#pragma once

#include "gl.h"

namespace gl
{
    class shader_program
    {
    public:
        static const char* shader_position_color;
        static const char* shader_texture_position;
        static const char* shader_texture_position_color;
        static const char* shader_texture_position_color_alpha;
		static const char* shader_font_position_color_alpha;

        shader_program(GLuint program_id);
        virtual ~shader_program();
        
        virtual void use(const math::mat4& transform) = 0;
    protected:
        GLint m_program_id = -1;
    };
    
    class shader_position_color : public shader_program
    {
    public:
        shader_position_color(GLint program_id);
        
        void use(const math::mat4& transform) override;
    };
    
    class shader_texture_position_color : public shader_program
    {
    public:
        shader_texture_position_color(GLint program_id);
        
        void use(const math::mat4& transform) override;
    };
}

