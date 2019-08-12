#pragma once
#include "Event.h"
#include "EventManager.h"

#include <string>

class EventManager;

class ISubject
{
public:
	using ID_Type = unsigned long long;
	ISubject()
		:m_owned_id{ GenerateID() }
	{}
	ISubject(const ISubject& i_other)
		:m_owned_id{i_other.m_owned_id}
	{}
	virtual ~ISubject();
	ID_Type GetUniqueIdentifier() const;
	virtual void Notify(const Event &i_event, const std::string &i_subtopic) const = 0;
private:
	static ID_Type GenerateID();
	const ID_Type m_owned_id;
};

class Subject : public ISubject
{
public:
	virtual void Notify(const Event& i_event, const std::string& i_subtopic) const override
	{
		EventManager::GetInstance().Notify(this, i_subtopic, i_event);
	}
};

