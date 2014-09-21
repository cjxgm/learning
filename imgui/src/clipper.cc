#include "clipper.hh"
#include <utility>

// make sure a <= b, otherwise, swap them to make it sure.
static void reorder(int& a, int& b)
{
	if (b < a) std::swap(a, b);
}




namespace imgui
{
	void clipper::set(int x1, int y1, int x2, int y2)
	{
		*stack.rbegin() = std::make_tuple(x1, y1, x2, y2);
	}

	void clipper::push(int x1, int y1, int x2, int y2)
	{
		clip(get(), x1, y1, x2, y2);
		stack.emplace_back(x1, y1, x2, y2);
	}

	void clipper::clip(xyxy boundary, int& x1, int& y1, int& x2, int& y2)
	{
		reorder(std::get<0>(boundary), x1);
		reorder(std::get<1>(boundary), y1);
		reorder(x2, std::get<2>(boundary));
		reorder(y2, std::get<3>(boundary));
	}
}

