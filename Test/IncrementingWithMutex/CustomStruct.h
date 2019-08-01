#pragma once
class CustomStruct
{
public:
	CustomStruct(float lhs = 0.0f, double rhs = 0.0);

	CustomStruct operator+(float value);

	float& GetFloatValue();
	double& GetDoubleValue();
private:
	float m_float_var;
	double m_double_var;
};

inline CustomStruct::CustomStruct(float lhs, double rhs)
	:m_float_var{lhs}
	,m_double_var{rhs}
{}

inline CustomStruct CustomStruct::operator+(float value)
{
	return { m_float_var + value, m_double_var + value };
}

float & CustomStruct::GetFloatValue()
{
	return m_float_var;
}

inline double & CustomStruct::GetDoubleValue()
{
	return m_double_var;
}
