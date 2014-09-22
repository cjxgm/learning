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


	template <class Hasher, class T>
		// requires Hashable<Hasher, T>()
	inline void hash_combine(size_t& seed, T const& x)
	{
		Hasher hash;
		seed ^= (hash(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
	}

	template <class T>
		// requires Hashable<T>()
	inline void hash_combine(size_t& seed, T const& x)
	{
		using std::hash;
		using hasher = hash<T>;
		hash_combine<hasher>(seed, x);
	}


	inline void merge(xyxy & dst, xyxy src)
	{
		reorder(dst.x1, src.x1);
		reorder(dst.y1, src.y1);
		reorder(src.x2, dst.x2);
		reorder(src.y2, dst.y2);
	}


	inline int downsample_floor(int x, int level)
	{
		return (x >> level) << level;
	}

	inline int downsample_ceil(int x, int level)
	{
		auto mask = (1 << level) - 1;
		return ((x >> level) + !!(x & mask)) << level;
	}

	inline xyxy downsample(xyxy const& r, int level)
	{
		return {
			downsample_floor(r.x1, level),
			downsample_floor(r.y1, level),
			downsample_ceil (r.x2, level),
			downsample_ceil (r.y2, level)
		};
	}


	// some hashers for quad types
	template <class Q>
		// requires Quad<Q>()
	struct quad_hash
	{
		size_t operator () (Q const& q) const
		{
			size_t seed = 0;
			hash_combine(seed, q[0]);
			hash_combine(seed, q[1]);
			hash_combine(seed, q[2]);
			hash_combine(seed, q[3]);
			return seed;
		}
	};

	using xyxy_hash = quad_hash<xyxy>;
	using xywh_hash = quad_hash<xywh>;
	using rgba_hash = quad_hash<rgba>;
}

