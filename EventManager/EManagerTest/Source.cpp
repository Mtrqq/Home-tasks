#include "..\EventManager\EventManager.h"
#include "..\EventManager\Subject.h"
#include "..\EventManager\Observer.h"

#include <iostream>

int main()
{
	IObserver* obs = new Observer;
	Subject subject;
	auto connection = EventManager::GetInstance().AddObserver(&subject, obs, "kuku");
	delete obs;
	subject.Notify(Event{}, "kuku");
}