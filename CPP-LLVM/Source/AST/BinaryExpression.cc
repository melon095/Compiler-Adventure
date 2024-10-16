#include <AST/BinaryExpression.hh>
#include <Lexer/Tokens.hh>

#include <magic_enum.hpp>

namespace K::AST
{
	BinaryOperator GetBinaryOperator(TokenID token)
	{
		switch(token)
		{
			case TokenID::Plus: return BinaryOperator::Add;
			case TokenID::Minus: return BinaryOperator::Sub;
			case TokenID::Star: return BinaryOperator::Mul;
			case TokenID::Slash: return BinaryOperator::Div;
			case TokenID::Less: return BinaryOperator::Less;
			default: throw std::runtime_error("Invalid binary operator");
		}
	}

	BinaryExpression::BinaryExpression(BinaryOperator op, ExpressionPtr lhs, ExpressionPtr rhs)
		: BaseExpression()
		, Op(op)
		, LHS(std::move(lhs))
		, RHS(std::move(rhs))
	{
	}

	BinaryExpression::BinaryExpression(TokenID op, ExpressionPtr lhs, ExpressionPtr rhs)
		: BinaryExpression(GetBinaryOperator(op), std::move(lhs), std::move(rhs))
	{
	}

	void BinaryExpression::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "BinaryExpression(" << magic_enum::enum_name(Op) << ')' << std::endl;

		LHS->Dump(indent + 2, os);
		RHS->Dump(indent + 2, os);
	}

	Codegen::CodegenResult BinaryExpression::Codegen(Codegen::CodegenContextPtr context) const
	{
		return Codegen::CodegenResult::NotImplemented(this->shared_from_this());
		// auto& builder = context->GetBuilder();

		// auto lhs = LHS->Codegen(context);
		// auto rhs = RHS->Codegen(context);

		// switch(Op)
		// {
		//     case BinaryOperator::Add:
		//         return builder.CreateFAdd(lhs, rhs, "addtmp");
		//     case BinaryOperator::Sub:
		//         return builder.CreateFSub(lhs, rhs, "subtmp");
		//     case BinaryOperator::Mul:
		//         return builder.CreateFMul(lhs, rhs, "multmp");
		//     case BinaryOperator::Div:
		//         return builder.CreateFDiv(lhs, rhs, "divtmp");
		//     case BinaryOperator::Less:
		//         lhs = builder.CreateFCmpULT(lhs, rhs, "cmptmp");
		//         return builder.CreateUIToFP(lhs, llvm::Type::getDoubleTy(context->GetContext()), "booltmp");
		//     default:
		//         throw std::runtime_error("Invalid binary operator");
		// }
	}
} // namespace K::AST
