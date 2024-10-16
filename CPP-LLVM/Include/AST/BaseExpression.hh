#pragma once

#include <IDumpable.hh>
#include <PtrUtils.hh>

#include <memory>
#include <ostream>

namespace K::AST
{
	class BaseExpression : virtual public IDumpable
	{
	  public:
		virtual ~BaseExpression() = default;
		virtual void Dump(int indent, std::ostream& os) const = 0;

		template<typename T> bool Is() const { return dynamic_cast<const T*>(this) != nullptr; }
		template<typename T> T* As() { return dynamic_cast<T*>(this); }
	};

	typedef std::unique_ptr<BaseExpression> ExpressionPtr;
} // namespace K::AST