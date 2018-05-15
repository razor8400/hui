#include "gl.h"
#include "shaders_manager.h"

namespace gl
{
    static GLuint vertex_array_object;
    static GLuint vertex_buffer_object;
    static GLuint index_buffer_object;
    
    static GLuint draw_calls = 0;
    static std::vector<std::string> error_messages;
    
    namespace vertex_attribute
    {
        static const int position = 0;
        static const int texture_position = 1;
        static const int color = 2;
    };
    
    bool init_gl()
    {
        glewExperimental = true;
        
        if (glewInit() != GLEW_OK)
            return false;
        
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        error_messages.push_back("init success");
        
        return true;
    }
    
    void enable_depth()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
    
	void generate_buffers()
	{
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
        
        glGenBuffers(1, &vertex_buffer_object);
        glGenBuffers(1, &index_buffer_object);
    }
    
	void clear_buffers()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteBuffers(1, &vertex_buffer_object);
        glDeleteBuffers(1, &index_buffer_object);
	}

	void compile_shaders()
    {
        auto& manager = shaders_manager::instance();
        manager.compile_default_shaders();
    }

    void clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    GLint compile_shader(const char* source, unsigned int shader)
    {
        GLint result = GL_FALSE;
        int info_log_length = -1;
        
        GLuint shader_id = glCreateShader(shader);
        
        error_messages.push_back(std::string("compile shader:") + std::string(source));
        
        glShaderSource(shader_id, 1, &source , NULL);
        glCompileShader(shader_id);
        
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        
        if (info_log_length > 0)
        {
            char* error_message = new char[info_log_length + 1];
            glGetShaderInfoLog(shader_id, info_log_length, NULL, error_message);
            
            error_messages.push_back(error_message);
            delete[] error_message;
        }
        
        return shader_id;
    }
    
    GLint create_gl_program(const char* vert, const char* frag)
    {
        GLint result = GL_FALSE;
        int info_log_length = -1;
        
        GLuint vertex_shader = compile_shader(vert, GL_VERTEX_SHADER);
        GLuint fragment_shader = compile_shader(frag, GL_FRAGMENT_SHADER);
        
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &result);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
       
        if (info_log_length > 0)
        {
            char* error_message = new char[info_log_length + 1];
            glGetProgramInfoLog(program, info_log_length, NULL, error_message);
            
            error_messages.push_back(error_message);
            delete[] error_message;
        }
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        
        return program;
    }

	GLuint load_texture(const unsigned char* data, int width, int height, GLuint format)
	{
		GLuint texture;
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
        
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return texture;
	}

	GLuint render_to_texture(int width, int height, GLuint format, const std::function<void()>& draw_func)
	{
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        GLuint frame_buffer;
        glGenFramebuffers(1, &frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
        
        GLuint render_buffer;
        glGenRenderbuffers(1, &render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            error_messages.push_back("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

        clear();
        
        glViewport(0, 0, width, height);
        
        if (draw_func)
            draw_func();
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        
        clear();
        
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        
        glDeleteFramebuffers(1, &frame_buffer);
        glDeleteRenderbuffers(1, &render_buffer);
        
		return texture;
	}
    
    void bind_texture(GLuint texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    
    void set_blend_func(GLenum source, GLenum destination)
    {
        glBlendFunc(source, destination);
    }
    
    void enable_blend()
    {
        glEnable(GL_BLEND);
    }
    
    void disable_blend()
    {
        glDisable(GL_BLEND);
    }
    
    void draw_vertices(const std::vector<v3f_c4f_t2f>& vertices, const std::vector<GLushort>& indices)
    {
        if (vertices.empty())
            return;
        
        static const GLsizei size = sizeof(v3f_c4f_t2f);
        
        enable_blend();

        glEnableVertexAttribArray(vertex_attribute::position);
        glEnableVertexAttribArray(vertex_attribute::texture_position);
        glEnableVertexAttribArray(vertex_attribute::color);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, size * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        
        glVertexAttribPointer(vertex_attribute::position, 3, GL_FLOAT, GL_FALSE, size, (GLvoid*)offsetof(v3f_c4f_t2f, vertice));
        glVertexAttribPointer(vertex_attribute::color, 4, GL_FLOAT, GL_FALSE, size, (GLvoid*)offsetof(v3f_c4f_t2f, color));
        glVertexAttribPointer(vertex_attribute::texture_position, 2, GL_FLOAT, GL_FALSE, size, (GLvoid*)offsetof(v3f_c4f_t2f, tex_coord));
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices[0], GL_STATIC_DRAW);
        
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_SHORT, NULL);
        
        glDisableVertexAttribArray(vertex_attribute::position);
        glDisableVertexAttribArray(vertex_attribute::texture_position);
        glDisableVertexAttribArray(vertex_attribute::color);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        disable_blend();
        
        ++draw_calls;
    }
    
    const std::vector<std::string>& get_errors()
    {
        return error_messages;
    }
    
    void clear_errors()
    {
        error_messages.clear();
    }
    
    int get_draw_calls()
    {
        return draw_calls;
    }

	void sub_image2d(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
	{
		bind_texture(texture);
		glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
	}
    
    void delete_texture(GLuint texture)
    {
        glDeleteTextures(1, &texture);
    }
    
    void draw_line(float x1, float y1, float x2, float y2)
    {
        const GLfloat vertices[] = {
            x1, y1,
            x2, y2
        };
        
        GLfloat colors[] = {
            1, 1, 1,
            1, 1, 1
        };
        
        glEnableVertexAttribArray(vertex_attribute::position);
        glEnableVertexAttribArray(vertex_attribute::color);
        
        GLuint vertex_buffer;
        
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        
        GLuint color_buffer;
        
        glGenBuffers(1, &color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glVertexAttribPointer(vertex_attribute::color, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_LINES, 0, 2);
        
		glDisableVertexAttribArray(vertex_attribute::position);
        glDisableVertexAttribArray(vertex_attribute::color);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteBuffers(1, &color_buffer);
        
        ++draw_calls;
    }
    
    void draw_rect(float x, float y, float width, float height)
    {
        draw_line(x, y, x + width, y);
        draw_line(x + width, y, x + width, y + height);
        draw_line(x + width, y + height, x, y + height);
        draw_line(x, y + height, x, y);
    }
}
