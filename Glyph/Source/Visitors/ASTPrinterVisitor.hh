#pragma once

#include <AST/AST.hh>
#include <Visitors/IASTVisitor.hh>

#include <ostream>

namespace Glyph
{
	class AstNode;

	class ASTPrinterVisitor : public IASTVisitor<void>
	{
	  public:
		ASTPrinterVisitor(std::ostream& stream);

#define DEFINE_VISIT_METHOD(name) void Visit##name(name& node) override;
		AST_NODE_LIST(DEFINE_VISIT_METHOD)
#undef DEFINE_VISIT_METHOD

	  private:
		void Incr();
		void Decr();

	  private:
		std::ostream& out;
		std::size_t ident;
	};
} // namespace Glyph
