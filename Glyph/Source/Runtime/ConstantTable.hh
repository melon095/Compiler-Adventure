#pragma once

#include <Runtime/Value.hh>

#include <absl/status/statusor.h>
#include <vector>

namespace Glyph
{
	typedef size_t ConstantTableIndex;

	class ConstantTable
	{
	  public:
		ConstantTable() = default;
		~ConstantTable() = default;

		ConstantTableIndex AddValue(Value value);
		[[nodiscard]] absl::StatusOr<const Value*> GetValue(ConstantTableIndex index) const;
		[[nodiscard]] size_t GetSize() const { return m_Values.size(); }

	  private:
		std::vector<Value> m_Values;
	};

} // namespace Glyph
