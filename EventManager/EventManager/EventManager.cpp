// EventManager.cpp : Defines the functions for the static library.
//
#include "pch.h"
#include "EventManager.h"
#include "Observer.h"

#include <algorithm>

ScopedConnection EventManager::AddObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string & i_subtopic)
{
	m_signal_map[ip_subject][i_subtopic].push_back(ip_observer);
	return ScopedConnection{ ip_subject, i_subtopic, ip_observer };
}

bool EventManager::HasObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string& i_subtopic)
{
	auto found_observers = FindObserversCollection(ip_subject, i_subtopic);
	if (found_observers == nullptr)
		return false;
	return std::find(found_observers->cbegin(), found_observers->cend(), ip_observer) != found_observers->cend();
}

void EventManager::RemoveObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string& i_subtopic)
{
	auto found_observers = FindObserversCollection(ip_subject, i_subtopic);
	if (found_observers)
	{
		found_observers->erase(std::remove(found_observers->begin(), found_observers->end(), ip_observer), found_observers->end());
	}
}

void EventManager::RemoveObserver(const ISubject* ip_subject, IObserver* ip_observer)
{
	for (auto& topic_pair : m_signal_map[ip_subject])
	{
		RemoveObserver(ip_subject, ip_observer, topic_pair.first);
	}
}

void EventManager::RemoveObserver(IObserver* ip_observer)
{
	for (auto& subject_pair : m_signal_map)
	{
		RemoveObserver(subject_pair.first, ip_observer);
	}
}

void EventManager::RemoveSubject(const ISubject* ip_subject)
{
	m_signal_map.erase(ip_subject);
}

void EventManager::Notify(const ISubject* ip_source, const std::string& i_subtopic, const Event& i_event)
{
	auto found_observers = FindObserversCollection(ip_source, i_subtopic);
	if (found_observers)
	{
		for (auto& observer : *found_observers)
		{
			// Protect from deleted ptrs ?
			observer->OnEvent(i_event);
		}
	}
}

std::vector<IObserver*>* EventManager::FindObserversCollection(const ISubject* ip_subject, const std::string& i_subtopic)
{
	auto topics = m_signal_map.find(ip_subject);
	if (topics == m_signal_map.end())
		return nullptr;
	auto observer_pointer = topics->second.find(i_subtopic);
	if (observer_pointer == topics->second.end())
		return nullptr;
	return &observer_pointer->second;
}

