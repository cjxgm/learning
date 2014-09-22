#pragma once
#include <utility>
#include <chrono>
#include <string>

namespace imgui
{
	struct benchmark
	{
		struct measure_guard
		{
			using value_type = benchmark;
			using value_ref  = value_type &;
			using self_type = measure_guard;
			using self_ref  = self_type &;
			using self_cref = self_type const&;
			using self_rref = self_type &&;

			measure_guard(value_ref v) : value{v}, owned{true} { value.start(); }
			~measure_guard() { if (owned) value.stop(); }

			measure_guard(self_rref other)
				: value{other.value}, owned{other.owned}
			{
				other.owned = false;
			}
			self_ref operator = (self_rref other)
			{
				self_type tmp{std::move(other)};
				std::swap(*this, tmp);
				return *this;
			}

			measure_guard(self_cref) = delete;
			self_ref operator = (self_cref) = delete;

		private:
			value_ref value;
			bool owned;
		};

		using clock = std::chrono::high_resolution_clock;
		using time_point = clock::time_point;
		using duration = clock::duration;
		using nanoseconds = std::chrono::nanoseconds;
		using ns_type = nanoseconds::rep;
		using ms_type = float;


		measure_guard measure() { return {*this}; }
		void start() { last = clock::now(); }
		void  stop() { dura = clock::now() - last; }
		ns_type ns() const { return std::chrono::duration_cast< nanoseconds>(dura).count(); }
		ms_type ms() const { return ns() * 1e-6f; }

	private:
		time_point last;
		duration dura;
	};
}

