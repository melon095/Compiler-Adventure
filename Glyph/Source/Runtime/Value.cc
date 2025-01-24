#include <Macros.hh>
#include <Runtime/Value.hh>

namespace Glyph
{
	Value::Value(double value)
		: m_As(value)
		, m_Type(Type::Number)
	{
	}

	Value::Value(bool value)
		: m_As(value)
		, m_Type(Type::Bool)
	{
	}

	Value::~Value() = default;

	Value& Value::operator=(const Value& value)
	{
		this->m_As = value.m_As;
		
		return *this;
	}

	bool Value::operator==(const Glyph::Value& value) const
	{
		switch(m_Type)
		{
			case Type::Bool:
				{
					return m_As.m_BoolRep == value.AsBool();
				}
				break;

			case Type::Number:
				{
					return m_As.m_NumberRep == value.AsNumber();
				}

			case Type::Null:
				{
					if(value.IsNull())
						return true;

					return false;
				}
			case Type::Object:
				{
					if(value.IsObject())
						return m_As.m_ObjRep == value.AsObject();

					return false;
				}

			default:
				{
					return false;
				}
		}
	}

	bool Value::AsBool() const
	{
		switch(m_Type)
		{
			case Type::Bool: return m_As.m_BoolRep;
			case Type::Number: return m_As.m_NumberRep != 0;
			default: return false;
		}
	}

	double Value::AsNumber() const
	{
		switch(m_Type)
		{
			case Type::Bool: return m_As.m_BoolRep ? 1 : 0;
			case Type::Number: return m_As.m_NumberRep;
			default: return 0;
		}
	}

	Object* Value::AsObject() const
	{
		// FIXME: Not implemented
		NOT_REACHABLE();

		return nullptr;
	}

	std::string Value::ToString() const
	{
		switch(m_Type)
		{
			case Type::Null: return "null";
			case Type::Number: return std::to_string(m_As.m_NumberRep);
			case Type::Bool: return m_As.m_BoolRep ? "true" : "false";
			default: return "null";
		}
	}
} // namespace Glyph
