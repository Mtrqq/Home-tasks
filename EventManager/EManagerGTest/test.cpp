
#include "..\\EventManager\EventManager.h"
#include "..\\EventManager\Subject.h"
#include "..\\EventManager\Observer.h"
#include "..\\EventManager\Event.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class MockedObserver : public IObserver
{
public:
	MOCK_METHOD1(OnEvent, void(const Event&));
};

TEST(EManagerTest, Should_Work_With_One_Observer)
{
	Subject sub;
	MockedObserver mockobs;
	EventManager::GetInstance().ConnectObserver(&sub, &mockobs, "example");
	EXPECT_CALL(mockobs, OnEvent(testing::_)).Times(1);
	sub.Notify(Event{}, "example");
}

TEST(EManagerTest, Should_Not_Notify_With_Scope_Closed)
{
	Subject sub;
	MockedObserver mockobs;
	EventManager::GetInstance().ScopedConnectObserver(&sub, &mockobs, "example");
	EXPECT_CALL(mockobs, OnEvent(testing::_)).Times(0);
	sub.Notify(Event{}, "example");
}

TEST(EManagerTest, Should_Not_Throw_After_Slot_Deletion)
{
	Subject sub;
	MockedObserver* mockobs = new MockedObserver;
	EventManager::GetInstance().ScopedConnectObserver(&sub, mockobs, "example");
	EXPECT_NO_THROW(sub.Notify(Event{}, "example"));
}

TEST(EManagerTest, Should_Work_With_Multiple_Signals_Slots)
{
	Subject sub1;
	Subject sub2;
	MockedObserver* obs1 = new MockedObserver;
	MockedObserver obs2;
	MockedObserver obs3;
	auto& manager = EventManager::GetInstance();
	manager.ConnectObserver(&sub1, obs1, "topic");
	manager.ConnectObserver(&sub1, &obs2, "topic");
	manager.ConnectObserver(&sub1, &obs3, "random");
	manager.ConnectObserver(&sub2, obs1, "topic");
	manager.ConnectObserver(&sub2, &obs3, "random");
	EXPECT_CALL(*obs1, OnEvent(testing::_)).Times(2);
	EXPECT_CALL(obs2, OnEvent(testing::_)).Times(1);
	EXPECT_CALL(obs3, OnEvent(testing::_)).Times(1);
	sub1.Notify(Event{}, "topic");
	sub2.Notify(Event{}, "topic");
	sub2.Notify(Event{}, "random");
}