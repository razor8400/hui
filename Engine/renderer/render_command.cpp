#include "common.h"
#include "render_command.h"

namespace engine
{
    void triangles_commad::add_vertice(const gl::v3f_c4f_t2f& vertice)
    {
        m_vertices.push_back(vertice);
    }
    
    void triangles_commad::execute(const math::mat4& world)
    {
        gl::set_blend_func(m_blend_func.source, m_blend_func.destination);
        gl::bind_texture(m_texture);
        
        if (m_program)
            m_program->use(world);
        
        gl::draw_vertices(m_vertices, indices());
    }
    
    void triangles_commad::reset()
    {
        m_vertices.clear();
    }
    
    std::shared_ptr<quads_command> quads_command::last_command;
    
    std::shared_ptr<quads_command> quads_command::create(int texture, const gl::blend_func& blend_func, const gl::shader_program_ptr& program)
    {
        if (last_command != nullptr)
        {
            if (last_command->m_texture == texture
                && last_command->m_blend_func.source == blend_func.source
                && last_command->m_blend_func.destination == blend_func.destination
                && last_command->m_program == program)
                return last_command;
        }
        
        auto command = std::make_shared<quads_command>();
        
        command->m_texture = texture;
        command->m_blend_func = blend_func;
        command->m_program = program;
        
        last_command = command;
        
        return command;
    }
    
    std::vector<unsigned short> quads_command::indices() const
    {
        auto size = m_vertices.size() * 6 / 4;
        std::vector<unsigned short> indices(size);
        
        for (unsigned short i = 0, index = 0; i < size; i += 6, index += gl::quad_size)
        {
            indices[i] = index;
            indices[i + 1] = index + 1;
            indices[i + 2] = index + 2;
            indices[i + 3] = index + 2;
            indices[i + 4] = index + 3;
            indices[i + 5] = index;
        }
        
        return indices;
    }
    
    std::shared_ptr<custom_command> custom_command::create(const std::function<void()>& handler)
    {
        auto command = std::make_shared<custom_command>();
        command->m_handler = handler;
        return command;
    }
    
    void custom_command::execute(const math::mat4& world)
    {
        if (m_handler)
            m_handler();
    }
    
    void custom_command::reset()
    {
        m_handler = nullptr;
    }
}
