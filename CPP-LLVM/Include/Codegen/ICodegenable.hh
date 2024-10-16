#pragma once

#include <Codegen/CodegenContext.hh>
#include <Codegen/CodegenResult.hh>

#include <memory>

namespace K::Codegen
{
	class ICodegenable
	{
	  public:
		virtual ~ICodegenable() = default;

		virtual CodegenResult Codegen(CodegenContextPtr context) const = 0;
	};
} // namespace K::Codegen
