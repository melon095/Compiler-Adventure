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
		auto builder = context->GetBuilder();

		auto* lhs = CODEGEN_CHECK(LHS->Codegen(context));
		auto* rhs = CODEGEN_CHECK(RHS->Codegen(context));

		llvm::Value* n = nullptr;

		switch(Op)
		{
			case BinaryOperator::Add:
				{
					n = builder->CreateFAdd(lhs, rhs, "addtmp");
				}
				break;

			case BinaryOperator::Sub:
				{
					n = builder->CreateFSub(lhs, rhs, "subtmp");
				}
				break;

			case BinaryOperator::Mul:
				{
					n = builder->CreateFMul(lhs, rhs, "multmp");
				}
				break;

			case BinaryOperator::Div:
				{
					n = builder->CreateFDiv(lhs, rhs, "divtmp");
				}
				break;

			case BinaryOperator::Less:
				{
					n = builder->CreateFCmpULT(lhs, rhs, "cmptmp");
					n = builder->CreateUIToFP(n, llvm::Type::getDoubleTy(context->GetContext()), "booltmp");
				}
				break;

			default: return Codegen::CodegenResult::Error(shared_from_this(), "Invalid binary operator");
		}

		context->GetValueStack().push(n);

		return Codegen::CodegenResult::Ok(shared_from_this(), n);
	}
} // namespace K::AST
