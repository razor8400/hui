#include "common.h"
#include "action.h"

#include "scripting/scripting.h"
#include "core/game_object.h"

namespace engine
{
    IMPLEMENT_TYPE_INFO(action)
    
    void action::update(float dt)
    {
        if (is_action_done())
            m_parent->remove_component(this);
    }
    
    IMPLEMENT_TYPE_INFO(targeted_action)
    
    targeted_action::~targeted_action()
    {

    }
    
    bool targeted_action::init(action* action, game_object* target)
    {
        if (!action || !target)
            return false;
        
        m_action = action;
        m_target = target;
        
        return true;
    }
    
    void targeted_action::start()
    {
        assert(m_parent);
        m_target->add_component(m_action);
    }
    
    IMPLEMENT_TYPE_INFO(action_instant)
    
    void action_instant::start()
    {
        m_handled = handle_action();
    }
    
    IMPLEMENT_TYPE_INFO(action_lua_callback)
    
    action_lua_callback::~action_lua_callback()
    {
        scripting::clear_ref(m_state, m_handler);
    }
    
    bool action_lua_callback::init(lua_State* state, int handler)
    {
        if (!state)
            return false;
        
        m_handler = handler;
        m_state = state;
        
        return true;
    }
    
    bool action_lua_callback::handle_action()
    {
        scripting::call_method(m_state, m_handler);
        
        return true;
    }
    
    void action_inverval::start()
    {
        m_time = 0;
    }
    
    void action_inverval::update(float dt)
    {
        if (m_duration > 0)
        {
            m_time += dt;
            step(std::max(0.0f, std::min(m_time / m_duration, 1.0f)));
        }
        
        action::update(dt);
    }
    
    IMPLEMENT_TYPE_INFO(action_sequence)
    
    action_sequence::~action_sequence()
    {

    }
    
    bool action_sequence::init(const vector<action*>& actions)
    {
        for (auto action : actions)
            m_actions.push_back(action);
        
        return true;
    }
    
    void action_sequence::append(action* action)
    {
        m_actions.push_back(action);
    }
    
    void action_sequence::start()
    {
        m_current_action = 0;
        start_next_action();
    }
    
    void action_sequence::update(float dt)
    {
        auto& action = m_actions.at(m_current_action);
        if (action->is_action_done())
        {
            ++m_current_action;
            start_next_action();
        }
        
        action::update(dt);
    }
    
    void action_sequence::start_next_action()
    {
        if (m_current_action < m_actions.size())
        {
            auto& action = m_actions.at(m_current_action);
            m_parent->add_component(action);
        }
    }
    
    IMPLEMENT_TYPE_INFO(action_list)
    
    action_list::~action_list()
    {

    }
    
    bool action_list::init(const vector<action*>& actions)
    {
        for (auto action : actions)
            m_actions.push_back(action);
        
        return true;
    }
    
    void action_list::append(action* action)
    {
        m_actions.push_back(action);
    }
    
    void action_list::update(float dt)
    {
        m_running_actions.lock([=]()
        {
            for (auto& action : m_running_actions)
            {
                if (action->is_action_done())
                    m_running_actions.erase(action);
            }
        });
        
        action::update(dt);
    }
    
    void action_list::start()
    {
        for (auto& action : m_actions)
        {
            m_parent->add_component(action);
            m_running_actions.push_back(action);
        }
    }
    
    IMPLEMENT_TYPE_INFO(action_delay)
    
    bool action_delay::init(float duration)
    {
        m_duration = duration;
        return true;
    }
    
    IMPLEMENT_TYPE_INFO(action_move)
    
    action_move* action_move::move(const math::vector3d& from, const math::vector3d& to, float duration)
    {
        auto action = ref::create<action_move>();
        
        action->m_from = from;
        action->m_to = to;
        action->m_duration = duration;
        
        return action;
    }
    
    action_move* action_move::move_to(const math::vector3d& to, float duration)
    {
        auto action = ref::create<action_move>();
        
        action->m_use_target_location = true;
        action->m_to = to;
        action->m_duration = duration;
        
        return action;
    }
    
    action_move* action_move::move_by(const math::vector3d& by, float duration)
    {
        auto action = ref::create<action_move>();
        
        action->m_move_by = true;
        action->m_by = by;
        action->m_duration = duration;
        
        return action;
    }
    
    void action_move::step(float s)
    {
        auto delta = m_to - m_from;

        m_parent->set_position(m_from + (delta * s));
    }
    
    void action_move::start()
    {
        assert(m_parent);
        
        if (m_use_target_location)
        {
            m_from = m_parent->get_position();
        }
        else
        {
            m_parent->set_position(m_from);
        }
        
        if (m_move_by)
            m_to = m_parent->get_position() + m_by;
        
        action_inverval::start();
    }
}
