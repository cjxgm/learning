#pragma once

#if LOG_SYSTEM == file_log
#include <fstream>
#endif

#if LOG_SYSTEM == stderr_log
#include <iostream>
#endif

namespace library
{
	namespace no_log
	{
		struct log {};
		template <class T>
		log& operator<<(log& l, T&&) { return l; }
	}

	namespace file_log
	{
		struct log
		{
			std::ofstream o{"./library.log"};
		};
		template <class T>
		log& operator<<(log& l, T&& what)
		{
			l.o << what;
			l.o.flush();
			return l;
		}
	}

	namespace stderr_log
	{
		struct log {};
		template <class T>
		log& operator<<(log& l, T&& what)
		{
			std::cerr << what;
			return l;
		}
	}

#ifndef LOG_SYSTEM
#define LOG_SYSTEM no_log
#endif

	namespace log_system = LOG_SYSTEM;
	log_system::log& log();
}

