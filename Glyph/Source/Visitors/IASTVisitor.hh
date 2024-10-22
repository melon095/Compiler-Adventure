#pragma once

#include <AST/AST.hh>

namespace Glyph
{
	/// @brief Base class for all AST visitors
	class IASTVisitor
	{
	  public:
		virtual ~IASTVisitor() = default;

#define DEFINE_VISIT_METHOD(name) virtual void Visit##name(name& node) = 0;
		AST_NODE_LIST(DEFINE_VISIT_METHOD)
#undef DEFINE_VISIT_METHOD

		void Visit(AstNode& node);
	};
} // namespace Glyph
