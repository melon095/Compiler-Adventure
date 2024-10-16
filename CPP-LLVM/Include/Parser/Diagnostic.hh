#pragma once

#include <string>
#include <vector>

namespace K
{
	struct Diagnostic
	{
		/// @brief The code associated with the diagnostic.
		std::string Code;

		/// @brief The message associated with the diagnostic.
		std::string Message;

		/// @brief The line number where the diagnostic occurred.
		std::size_t Line;

		/// @brief The column number where the diagnostic occurred.
		std::size_t Column;
	};
} // namespace K

std::ostream& operator<<(std::ostream& os, const K::Diagnostic& diagnostic);