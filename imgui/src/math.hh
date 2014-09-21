#pragma once
#include <utility>
#include "quad.hh"

namespace imgui
{
	namespace intersect
	{
		inline bool segment(int a1, int a2, int b1, int b2)
		{
			return (b1 <= a2 && a1 <= b2);
		}

		inline bool rectangle(xyxy const& a, xyxy const& b)
		{
			return (segment(a.x1, a.x2, b.x1, b.x2) &&
					segment(a.y1, a.y2, b.y1, b.y2));
		}
	}


	// make sure a <= b
	// otherwise, swap them to make it sure.
	template <class T>
		// requires LessThanComparable<T>() &&
		// 			Swappable<T>()
	inline void reorder(T& a, T& b)
	{
		using std::swap;
		if (b < a) swap(a, b);
	}


	// clip the rectangle "r" into the rectangle "boundary"
	// return false when "r" is completely outside "boundary"
	// return true otherwise
	inline bool clip(xyxy const& boundary, xyxy & r)
	{
		if (!intersect::rectangle(boundary, r)) return false;
		auto b = boundary;
		reorder(b.x1, r.x1);
		reorder(b.y1, r.y1);
		reorder(r.x2, b.x2);
		reorder(r.y2, b.y2);
		return true;
	}


	template <class T>
		// requires Hashable<T>()
	inline void hash_combine(size_t& seed, T const& x)
	{
		std::hash<T> hash;
		seed ^= (hash(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
	}


	inline void merge(xyxy & dst, xyxy src)
	{
		reorder(dst.x1, src.x1);
		reorder(dst.y1, src.y1);
		reorder(src.x2, dst.x2);
		reorder(src.y2, dst.y2);
	}
}
