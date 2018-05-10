#include "common.h"
#include "texture_atlas.h"

#include "utils/file_utils.h"
#include "nlohmann/json.hpp"

namespace engine
{
	std::shared_ptr<texture_atlas> texture_atlas::load_from_file(const std::string & file_name)
	{
		auto data = file_utils::file_data();

		logger() << "[texture atlas] load:" << file_name;

		if (file_utils::read_file(file_name, &data.buffer, &data.size))
		{
			auto atlas = std::make_shared<texture_atlas>();

			if (atlas->load(data.buffer, data.size))
				return atlas;
		}

		logger() << "[texture atlas] load error:" << file_name;

		return std::shared_ptr<texture_atlas>();
	}

	bool texture_atlas::load(const unsigned char* data, size_t size)
	{
		try
		{
			auto buffer = std::string(data, data + size);

			logger() << "[texture atlas] parse json:\n" << buffer;

			auto json = nlohmann::json::parse(buffer);

			m_texture = resources_manager::instance().load_resource_from_file<texture2d>(json["meta"]["image"]);

			auto frames = json["frames"];

			for (auto data : frames)
			{
				auto frame = sprite_frame();
				auto rect = data["frame"];

				auto x = rect["x"].get<float>();
				auto y = rect["y"].get<float>();
				auto w = rect["w"].get<float>();
				auto h = rect["h"].get<float>();
				
				auto source_size = data["sourceSize"];
				auto file_name = data["filename"].get<std::string>();

				frame.frame = math::rect(x, y, w, h);
				frame.rotated = data["rotated"].get<bool>();
				frame.source_size = math::vector2d(source_size["w"].get<float>(), source_size["h"].get<float>());

				m_frames[file_name] = frame;
			}

			return true;
		}
		catch (const nlohmann::json::parse_error& e)
		{
			logger() << "[texture atlas] " << e.what();
		}
		catch (const nlohmann::json::type_error& e)
		{
			logger() << "[texture atlas] " << e.what();
		}

		return false;
	}

	bool texture_atlas::get_frame(const std::string& file_name, sprite_frame* frame) const
	{
		auto it = m_frames.find(file_name);

		if (it != m_frames.end())
		{
			*frame = it->second;
			return true;
		}

		logger() << "[texture atlas] can't find frame by name:" << file_name;
 		
		return false;
	}
}