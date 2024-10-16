#pragma once

#include <Codegen/ICodegenable.hh>
#include <IDumpable.hh>
#include <PtrUtils.hh>

#include <memory>
#include <ostream>

namespace K::AST
{
	struct DebugInfo
	{
		int Line;
		int Column;
	};

	struct ExpressionData
	{
		DebugInfo Debug;
	};

	class BaseExpression : virtual public IDumpable,
						   virtual public Codegen::ICodegenable,
						   public std::enable_shared_from_this<BaseExpression>
	{
	  public:
		BaseExpression();
		virtual ~BaseExpression() = default;

		virtual void Dump(int indent, std::ostream& os) const = 0;
		virtual Codegen::CodegenResult Codegen(Codegen::CodegenContextPtr context) const = 0;

		template<typename T> bool Is() const { return dynamic_cast<const T*>(this) != nullptr; }
		template<typename T> T* As() { return dynamic_cast<T*>(this); }

		const ExpressionData& GetData() const { return m_Data; }
		ExpressionData& GetData() { return m_Data; }

		void SetDebugInfo(int line, int column) { m_Data.Debug = {line, column}; }

	  protected:
		ExpressionData m_Data;
	};

	typedef std::shared_ptr<BaseExpression> ExpressionPtr;
} // namespace K::AST