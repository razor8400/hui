#pragma once

namespace gl
{
    namespace shaders
    {
        static const char* shader_position_color_frag =
        R"(
        #version 330 core
        out vec3 fragment_color;
        in vec3 color;
        void main()
        {
            fragment_color = color;
        })";

        static const char* shader_position_color_vert =
        R"(
        #version 330 core
        layout(location = 0) in vec3 vertex_position_modelspace;
        layout (location = 2) in vec3 vertex_color;
        uniform mat4 mvp;
        out vec3 color;
        void main()
        {
            gl_Position = mvp * vec4(vertex_position_modelspace, 1);
            color = vertex_color;
        })";

		static const char* shader_texture_position_color_alpha_frag =
        R"(
        #version 330 core
        in vec2 uv;
        in vec4 color;
        out vec4 fragment_color;
        uniform sampler2D sampler2d;
        void main()
        {
            fragment_color = texture(sampler2d, uv).rgba * color;
        })";

		static const char* shader_font_position_color_alpha_frag =
        R"(
        #version 330 core
        in vec2 uv;
        in vec4 color;
        out vec4 fragment_color;
        uniform sampler2D sampler2d;
        void main()
        {
            fragment_color = vec4(1, 1, 1, texture(sampler2d, uv).r) * color;
        })";
        
        static const char* shader_texture_position_color_alpha_vert =
        R"(
        #version 330 core
        layout(location = 0) in vec3 vertex_position_modelspace;
        layout(location = 1) in vec2 vertex_uv;
        layout(location = 2) in vec4 vertex_color;
        out vec2 uv;
        out vec4 color;
        uniform mat4 mvp;
        void main()
        {
            gl_Position = mvp * vec4(vertex_position_modelspace, 1);
            uv = vertex_uv;
            color = vertex_color;
        })";
        
        static const char* shader_texture_position_frag =
        R"(
        #version 330 core
        in vec2 uv;
        out vec3 fragment_color;
        uniform sampler2D sampler2d;
        void main()
        {
            fragment_color = texture(sampler2d, uv).rgb;
        })";
        
        static const char* shader_texture_position_vert =
        R"(
        #version 330 core
        layout(location = 0) in vec3 vertex_position_modelspace;
        layout(location = 1) in vec2 vertex_uv;
        out vec2 uv;
        uniform mat4 mvp;
        void main()
        {
            gl_Position = mvp * vec4(vertex_position_modelspace, 1);
            uv = vertex_uv;
        })";
        
        static const char* shader_texture_position_color_frag =
        R"(
        #version 330 core
        in vec2 uv;
        in vec3 color;
        out vec3 fragment_color;
        uniform sampler2D sampler2d;
        void main()
        {
            fragment_color = texture(sampler2d, uv).rgb * color;
        })";

		static const char* shader_texture_position_color_vert =
        R"(
        #version 330 core
        layout(location = 0) in vec3 vertex_position_modelspace;
        layout(location = 1) in vec2 vertex_uv;
        layout(location = 2) in vec3 vertex_color;
        out vec2 uv;
        out vec3 color;
        uniform mat4 mvp;
        void main()
        {
            gl_Position = mvp * vec4(vertex_position_modelspace, 1);
            uv = vertex_uv;
            color = vertex_color;
        })";
        
        static const char* mvp = "mvp";
        static const char* texture_sampler2d = "sampler2d";
    }
}
