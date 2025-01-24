#include "ConstantTable.hh"

namespace Glyph
{
	ConstantTableIndex ConstantTable::AddValue(Glyph::Value value)
	{
		for(size_t i = 0; i < m_Values.size(); i++)
		{
			if(m_Values[i] == value)
			{
				return i;
			}
		}

		m_Values.push_back(std::move(value));

		return m_Values.size() - 1;
	}

	absl::StatusOr<const Value*> ConstantTable::GetValue(ConstantTableIndex index) const
	{
		if(index >= m_Values.size())
		{
			return absl::InvalidArgumentError("ConstantTable::GetValue: Index out of bounds");
		}

		return &m_Values[index];
	}
} // namespace Glyph
