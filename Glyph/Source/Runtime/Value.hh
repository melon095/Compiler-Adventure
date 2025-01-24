#pragma once

#include <string>

namespace Glyph
{
	typedef void Object; // FIXME: Use a proper object representation

	class Value
	{
	  public:
		enum class Type
		{
			Bool,
			Number,
			Object,
			Null
		};

	  public:
		Value() = delete; // FIXME: Null?
		explicit Value(double value);
		explicit Value(bool value);
		~Value();

		Value(const Value& value) = default;
		Value& operator=(const Value& value);
		bool operator==(const Value& value) const;

		[[nodiscard]] Type Type() const { return m_Type; }
		[[nodiscard]] bool IsBool() const { return m_Type == Type::Bool; }
		[[nodiscard]] bool IsNumber() const { return m_Type == Type::Number; }
		[[nodiscard]] bool IsNull() const { return m_Type == Type::Null; }
		[[nodiscard]] bool IsObject() const { return m_Type == Type::Object; }

		[[nodiscard]] bool AsBool() const;
		[[nodiscard]] double AsNumber() const;
		[[nodiscard]] Object* AsObject() const;

		std::string ToString() const;

	  private:
		union as
		{
			explicit as(bool value)
				: m_BoolRep(value)
			{
			}
			explicit as(double value)
				: m_NumberRep(value)
			{
			}
			explicit as(Object* obj)
				: m_ObjRep(obj)
			{
			}

			bool m_BoolRep;
			double m_NumberRep;
			Object* m_ObjRep;
		} m_As;

		enum Type m_Type;
	};
} // namespace Glyph
