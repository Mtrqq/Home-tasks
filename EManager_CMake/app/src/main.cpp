#include <iostream>
#include <memory>

#include <EManager/EventManager.h>
#include <EManager/Subject.h>
#include <EManager/Observer.h>

int main()
{
	std::unique_ptr<Subject> subject{ new Subject };
	std::unique_ptr<Observer> observer{ new Observer };
	std::cout << EventManager::GetInstance().HasObserver(subject.get(), observer.get(), "topic");
	std::cin.get();
}