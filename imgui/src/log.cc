#include "log.hh"

namespace library
{
	log_system::log& log()
	{
		static log_system::log l;
		return l;
	}
}

