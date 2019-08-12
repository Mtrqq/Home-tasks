#pragma once
#include "Event.h"
#include "EventManager.h"

#include <iostream>

class IObserver
{
public:
	virtual void OnEvent(const Event &i_event) = 0;
	virtual ~IObserver()
	{
		EventManager::GetInstance().DisconnectObserver(this);
	}
};

class Observer : public IObserver
{
public:
	virtual void OnEvent(const Event& i_event) override;
};

