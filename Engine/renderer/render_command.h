#pragma once

namespace engine
{
    class render_command
    {
    public:
        virtual void execute(const math::mat4& world) = 0;
        virtual void reset() = 0;
    };
    
    typedef std::shared_ptr<render_command> render_command_ptr;
    
    class triangles_commad : public render_command
    {
    public:
        void add_vertice(const gl::v3f_c4f_t2f& vertice);
        void execute(const math::mat4& world) override;
        void reset() override;
    protected:
        virtual std::vector<unsigned short> indices() const = 0;
    protected:
        int m_texture = -1;
        gl::blend_func m_blend_func;
        gl::shader_program_ptr m_program;
        std::vector<gl::v3f_c4f_t2f> m_vertices;
    };
    
    class quads_command : public triangles_commad
    {
    public:
        static std::shared_ptr<quads_command> create(int texture, const gl::blend_func& blend_func, const gl::shader_program_ptr& program);
    private:
        std::vector<unsigned short> indices() const override;
    private:
        static std::shared_ptr<quads_command> last_command;
    };
    
    class custom_command : public render_command
    {
    public:
        static std::shared_ptr<custom_command> create(const std::function<void()>& handler);
        
        void execute(const math::mat4& world) override;
        void reset() override;
    private:
        std::function<void()> m_handler;
    };
}
