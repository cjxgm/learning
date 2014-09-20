#pragma once

namespace imgui
{
	struct state
	{
		struct
		{
			int x;
			int y;
			bool down;
		}
		mouse = { 0, 0, false };
	};
}

