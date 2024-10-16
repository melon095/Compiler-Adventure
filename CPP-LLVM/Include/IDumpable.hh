#pragma once

#include <ostream>

namespace K
{
	class IDumpable
	{
	  public:
		virtual ~IDumpable() = default;

		virtual void Dump(int indent, std::ostream& os) const = 0;
	};
} // namespace K