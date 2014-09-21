#pragma once
// rectangle merger
#include <list>
#include "quad.hh"

namespace imgui
{
	struct merger
	{
		using list = std::list<xyxy>;

		void add(xyxy const& r) { rects.push_back(r); }
		void merge();

		auto begin() { return rects.begin(); }
		auto   end() { return rects.  end(); }

	private:
		list rects;
	};
}

