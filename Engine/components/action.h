#pragma once

#include "component.h"

struct lua_State;

namespace engine
{
    class action : public component
    {
        DECLARE_CLASS
    public:
        virtual void update(float dt) override;
        virtual void stop() override {};
        virtual bool is_action_done() const = 0;
        
        template<class T, class ...Args>
        static T* create(Args... args);
    };
    
    template<class T, class ...Args>
    T* action::create(Args... args)
    {
        auto action = ref::create<T>();
        
        if (action->init(args...))
            return action;
        
        return nullptr;
    }
    
    class targeted_action : public action
    {
        DECLARE_CLASS
    public:
        bool init(action* action, game_object* target);
        
        ~targeted_action();
        
        void start() override;
        
        bool is_action_done() const override { return m_action->is_action_done(); }
    private:
        pointer<action> m_action = nullptr;
        game_object* m_target = nullptr;
    };
    
    class action_instant : public action
    {
        DECLARE_CLASS
    public:
        void start() override;
        
        bool is_action_done() const override { return m_handled; }
    protected:
        virtual bool handle_action() = 0;
    private:
        bool m_handled = false;
    };
    
    class action_lua_callback : public action_instant
    {
        DECLARE_CLASS
    public:
        ~action_lua_callback();
        
        bool init(lua_State* state, int handler);
        bool handle_action() override;
    private:
        lua_State* m_state;
        int m_handler;
    };
    
    class action_inverval : public action
    {
        DECLARE_CLASS
    public:
        virtual void start() override;
        virtual void update(float dt) override;
        
        float get_duration() const { return m_duration; }
        bool is_action_done() const override { return m_time >= m_duration; }
    protected:
        virtual void step(float s) = 0;
    protected:
        float m_time = 0;
        float m_duration = 0;
    };
    
    class action_sequence : public action
    {
        DECLARE_CLASS
    public:
        ~action_sequence();
        
        bool init() { return true; }
        bool init(const vector<action*>& actions);
        
        void append(action* action);
        void update(float dt) override;
        
        bool is_action_done() const override { return m_current_action >= m_actions.size(); }
    private:
        void start() override;
        void start_next_action();
    private:
        vector<pointer<action>> m_actions;
        size_t m_current_action = 0;
    };
    
    class action_list : public action
    {
        DECLARE_CLASS
    public:
        ~action_list();
        
        bool init() { return true; }
        bool init(const vector<action*>& actions);
        
        void append(action* action);
        void update(float dt) override;
        
        bool is_action_done() const override { return m_running_actions.empty(); }
    private:
        void start() override;
    private:
        vector<pointer<action>> m_actions;
        vector<pointer<action>> m_running_actions;
    };
    
    class action_delay : public action_inverval
    {
        DECLARE_CLASS
    public:
        bool init(float duration);
    private:
        void step(float s) {};
    };
    
    class action_move : public action_inverval
    {
        DECLARE_CLASS
    public:
        static action_move* move(const math::vector3d& from, const math::vector3d& to, float duration);
        static action_move* move_to(const math::vector3d& to, float duration);
        static action_move* move_by(const math::vector3d& by, float duration);
    protected:
        void step(float s) override;
        void start() override;
    protected:
        math::vector3d m_from;
        math::vector3d m_to;
        math::vector3d m_by;
        
        bool m_move_by = false;
        bool m_use_target_location = false;
    };
}
