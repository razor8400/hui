#pragma once

#include <string>
#include <memory>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <set>
#include <numeric>
#include <functional>
#include <queue>
#include <cstdarg>
#include <assert.h>

#include "gl/libgl.h"
#include "math/libmath.h"

#include "core/type_info.h"

#include "utils/logger.h"
#include "utils/vector.h"
#include "utils/pointer.h"

#include "config.h"

namespace engine
{
    class ref;
    class renderer;
    
    class sprite;
    class label;
    
	class scene;
	class game_object;
    class component;

    class resource;
    class texture2d;
    class font_ttf;
    class script;
    
    class touch_listener;
    class render_command;
    
    typedef std::shared_ptr<resource> resource_ptr;
    typedef std::shared_ptr<texture2d> texture2d_ptr;
    typedef std::shared_ptr<script> script_ptr;
    typedef std::shared_ptr<font_ttf> font_ttf_ptr;
    
    typedef std::shared_ptr<render_command> render_command_ptr;

	enum projection_mode
	{
		ortographic, perspective
	};

	enum class vertical_text_alignment
	{
		left,
		center,
		right
	};

	enum class horisontal_text_alignment
	{
		top,
		center,
		bottom
	};
	
    static const std::string empty_string;
    
	static std::string vector3d_to_string(const math::vector3d& v3)
	{
		std::stringstream ss;

		ss << "(" << v3.x << ", "  << v3.y << ", " << v3.z << ")";

		return ss.str();
	}

	static std::string projection_mode_to_string(projection_mode mode)
	{
		if (mode == perspective)
		{
			return "perspective";
		}
		else if (mode == ortographic)
		{
			return "ortographic";
		}

		return empty_string;
	}
}

#include "utils/pointer.inl"
#include "utils/vector.inl"
#include "scripting/scripting.inl"

