// EventManager.cpp : Defines the functions for the static library.
//
#include <EManager/EventManager.h>
#include <EManager/Observer.h>
#include <EManager/Subject.h>

#include <algorithm>

void EventManager::ConnectObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string& i_subtopic)
{
	m_signal_map[ip_subject->GetUniqueIdentifier()][i_subtopic].push_back(ip_observer);
}

ScopedConnection EventManager::ScopedConnectObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string & i_subtopic)
{
	ConnectObserver(ip_subject, ip_observer, i_subtopic);
	return ScopedConnection{ ip_subject, i_subtopic, ip_observer };
}	

bool EventManager::HasObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string& i_subtopic)
{
	auto found_observers = FindObserversCollection(ip_subject->GetUniqueIdentifier(), i_subtopic);
	if (found_observers == nullptr)
		return false;
	return std::find(found_observers->cbegin(), found_observers->cend(), ip_observer) != found_observers->cend();
}

void EventManager::DisconnectObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string& i_subtopic)
{
	auto found_observers = FindObserversCollection(ip_subject->GetUniqueIdentifier(), i_subtopic);
	if (found_observers)
	{
		found_observers->erase(std::remove(found_observers->begin(), found_observers->end(), ip_observer), found_observers->end());
	}
}

void EventManager::DisconnectObserver(const ISubject* ip_subject, IObserver* ip_observer)
{
	for (auto& topic_pair : m_signal_map[ip_subject->GetUniqueIdentifier()])
	{
		DisconnectObserver(ip_subject, ip_observer, topic_pair.first);
	}
}

void EventManager::DisconnectObserver(IObserver* ip_observer)
{
	for (auto& subject_pair : m_signal_map)
	{
		for (auto& topic_pair : subject_pair.second)
		{
			auto& observers_vector = topic_pair.second;
			observers_vector.erase(std::remove(observers_vector.begin(), observers_vector.end(), ip_observer),
								   observers_vector.end());
		}
	}
}

void EventManager::RemoveSubject(const ISubject* ip_subject)
{
	m_signal_map.erase(ip_subject->GetUniqueIdentifier());
}

void EventManager::Notify(const ISubject* ip_source, const std::string& i_subtopic, const Event& i_event)
{
	auto found_observers = FindObserversCollection(ip_source->GetUniqueIdentifier(), i_subtopic);
	if (found_observers)
	{
		for (auto& observer : *found_observers)
		{
			// Protect from deleted ptrs ?
			observer->OnEvent(i_event);
		}
	}
}

std::vector<IObserver*>* EventManager::FindObserversCollection(ID_Type i_subject_id, const std::string& i_subtopic)
{
	auto topics = m_signal_map.find(i_subject_id);
	if (topics == m_signal_map.end())
		return nullptr;
	auto observer_pointer = topics->second.find(i_subtopic);
	if (observer_pointer == topics->second.end())
		return nullptr;
	return &observer_pointer->second;
}

