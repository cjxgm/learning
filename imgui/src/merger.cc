#include <algorithm>
#include "merger.hh"
#include "math.hh"


namespace imgui
{
	bool merger::collapse(xyxy & r, iterator it)
	{
		auto rit = std::remove_if(it, rects.end(),
				[&r](xyxy const& other) {
					if (!intersect::rectangle(r, other)) return false;
					math::merge(r, other);
					return true;
				});
		if (rit == rects.end()) return false;
		rects.erase(rit, rects.end());
		return true;
	}

	bool merger::merge_once()
	{
		for (auto it=rects.begin(); it != rects.end(); ++it)
			if (collapse(*it, std::next(it)))
				return true;
		return false;
	}
}

