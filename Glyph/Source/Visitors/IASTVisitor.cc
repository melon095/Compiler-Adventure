#include <Visitors/IASTVisitor.hh>

namespace Glyph
{
#define VISIT_CASE(name)                                                                                               \
	case NodeType::name: Visit##name(dynamic_cast<name&>(node)); break;

	void IASTVisitor<void>::Visit(AstNode& node)
	{
		switch(node.GetType())
		{
			AST_NODE_LIST(VISIT_CASE)
		}
	}
} // namespace Glyph
