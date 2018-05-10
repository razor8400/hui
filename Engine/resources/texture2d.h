#pragma once

#include "resource.h"

namespace engine
{
    class texture2d : public resource
	{
    public:
        static std::shared_ptr<texture2d> load_from_file(const std::string& file_name);
        
        texture2d(int width, int height, int format);
        virtual ~texture2d();
        
        bool load(const unsigned char* data, size_t size) override;

        int get_width() const { return m_width; }
        int get_height() const { return m_height; }
        
        int get_texture_id() const { return m_texture_id; }
		void set_texture_id(int texture_id) { m_texture_id = texture_id; }
        
        void draw(const math::mat4& t);
    private:
		int m_width = 0;
		int m_height = 0;
        int m_format = 0;
        int m_texture_id = -1;
	};
}
