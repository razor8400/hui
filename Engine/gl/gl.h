#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <map>

#include "GLEW/glew.h"
#include "math/libmath.h"

namespace gl
{
    bool init_gl();

    void enable_depth();
	void generate_buffers();
    void clear_buffers();
    void compile_shaders();
    void clear();
    
    GLint create_gl_program(const char* vert, const char* frag);
	GLuint load_texture(const unsigned char* data, int width, int height, GLuint format);
    GLuint render_to_texture(int width, int height, GLuint format, const std::function<void()>& draw_func);
    
    void bind_texture(GLuint texture);
    void set_blend_func(GLenum source, GLenum destination);
    void draw_texture2d(const std::vector<math::vector2d>& vertices, const std::vector<math::vector2d>& uv);
	void draw_texture2d(const std::vector<math::vector2d>& vertices, const std::vector<math::vector2d>& uv, const std::vector<math::vector4d>& colors);
    void delete_texture(GLuint texture);
    
    void draw_line(float x1, float y1, float x2, float y2);
    void draw_rect(float x, float y, float width, float height);
    void draw_solid_rect(float x, float y, float width, float height, const math::vector3d& color);
    
    const std::vector<std::string>& get_errors();
    void clear_errors();
    int get_draw_calls();
    
	void sub_image2d(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

    struct blend_func
    {
        GLenum source;
        GLenum destination;
    };
}
