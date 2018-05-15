#pragma once

#include "texture_protocol.h"

namespace engine
{
    class sprite : public texture_protocol
    {
        struct quad
        {
            std::vector<math::vector2d> vertices;
            std::vector<math::vector2d> uv;
            std::vector<math::vector4d> colors;
        };
        
        DECLARE_CLASS;
    public:
        bool init() override;
        bool init(const std::string& file_name);
		bool init(const texture2d_ptr& texture);
        bool init(const texture2d_ptr& texture, const math::rect& rect);
		bool init(const std::string& atlas_name, const std::string& file_name);

        void set_texture(const std::string& file_name);
        void set_texture(const texture2d_ptr& texture);
        void set_texture(const texture2d_ptr& texture, const math::rect& rect);

		const math::rect& get_texture_rect() const { return m_texture_rect; }
        void set_texture_rect(const math::rect& texture_rect) { m_texture_rect = texture_rect; update_vertices(); }
    private:
        void update_vertices();
    private:
		math::rect m_texture_rect;

		bool m_rotated = false;
    };
}
