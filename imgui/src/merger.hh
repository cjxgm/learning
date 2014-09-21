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
		void merge() { while (merge_once()) {} }

		auto begin() { return rects.begin(); }
		auto   end() { return rects.  end(); }

	private:
		list rects;

		using iterator = list::iterator;
		bool collapse(xyxy & r, iterator it);
		bool merge_once();
	};
}

