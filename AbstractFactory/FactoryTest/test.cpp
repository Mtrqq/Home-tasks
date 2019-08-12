#include "pch.h"

class AbstractClass
{
public:
	virtual int function() const = 0;
};

class OverrideClass : public AbstractClass
{
public:
	virtual int function() const override
	{
		return 5;
	}
};

auto& factory = InterfaceFactory::Instance();

TEST(AbstractFabricTest, Should_Work_With_Simple_Case) {
	auto connection = factory.ScopedRegister<AbstractClass, OverrideClass>();
	EXPECT_EQ(5, factory.Create<AbstractClass>()->function());
}

TEST(AbstractFabricTest, Should_Be_Unregistered_After_Scoped_Connection_Destruction)
{
	{
		auto short_time_connection = factory.ScopedRegister<AbstractClass, OverrideClass>();
	}
	EXPECT_EQ(false, factory.IsRegistered<AbstractClass>());
}

TEST(AbstractFabricTest, Should_Throw_With_Unregistered_Type)
{
	EXPECT_THROW(factory.Create<AbstractClass>(), std::runtime_error);
}