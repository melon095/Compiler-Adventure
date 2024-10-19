#pragma once

namespace Glyph
{
	/// @brief Base class for all AST visitors
	class IASTVisitor
	{
	  public:
		virtual ~IASTVisitor() = default;
	};
} // namespace Glyph