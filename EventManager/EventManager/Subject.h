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
		:m_owned_id{g_id_generator++}
	{}
	ISubject(const ISubject& i_other)
		:m_owned_id{i_other.m_owned_id}
	{}
	ID_Type GetUniqueIdentifier() const
	{
		return m_owned_id;
	}
	virtual void Notify(const Event &i_event, const std::string &i_subtopic) const = 0;
	virtual ~ISubject()
	{
		EventManager::GetInstance().RemoveSubject(this);
	}
private:
	static ID_Type g_id_generator;
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

