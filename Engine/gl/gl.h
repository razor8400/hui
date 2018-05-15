#pragma once

#include <array>
#include <vector>
#include <memory>
#include <functional>
#include <map>

#include "GLEW/glew.h"
#include "math/libmath.h"

namespace gl
{
    struct v3f_c4f_t2f;
    
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
    
    void draw_vertices(const std::vector<v3f_c4f_t2f>& vertices, const std::vector<GLushort>& indices);

    void delete_texture(GLuint texture);
    
    void draw_line(float x1, float y1, float x2, float y2);
    void draw_rect(float x, float y, float width, float height);
    
    const std::vector<std::string>& get_errors();
    void clear_errors();
    int get_draw_calls();
    
	void sub_image2d(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

    struct blend_func
    {
        GLenum source;
        GLenum destination;
    };
    
    struct v3f_c4f_t2f
    {
        math::vector3d vertice;
        math::vector4d color = math::vector4d::one;
        math::vector2d tex_coord;
    };
    
    static const int quad_size = 4;
    
    static const int bottom_left = 0;
    static const int bottom_right = 1;
    static const int top_right = 2;
    static const int top_left = 3;
    
    typedef std::array<v3f_c4f_t2f, quad_size> v3f_c4f_t2f_quad;
}
