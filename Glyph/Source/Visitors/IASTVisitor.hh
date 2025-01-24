#pragma once

#include <AST/AST.hh>

namespace Glyph
{
	/// @brief Base class for all AST visitors
	template<typename TReturnType = void> class IASTVisitor
	{
	  public:
		virtual ~IASTVisitor() = default;

#define DEFINE_VISIT_METHOD(name) virtual TReturnType Visit##name(name& node) = 0;
		AST_NODE_LIST(DEFINE_VISIT_METHOD)
#undef DEFINE_VISIT_METHOD

		virtual TReturnType Visit(AstNode& node) = 0;
	};

	template<> class IASTVisitor<void>
	{
	  public:
		virtual ~IASTVisitor() = default;

#define DEFINE_VISIT_METHOD(name) virtual void Visit##name(name& node) = 0;
		AST_NODE_LIST(DEFINE_VISIT_METHOD)
#undef DEFINE_VISIT_METHOD

		virtual void Visit(AstNode& node);
	};
} // namespace Glyph
