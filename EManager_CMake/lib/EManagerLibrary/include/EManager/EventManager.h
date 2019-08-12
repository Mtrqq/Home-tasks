#pragma once
#include "ScopedConnection.h"
#include "Event.h"

#include <unordered_map>
#include <vector>
#include <string>

class ISubject;
class IObserver;

class EventManager
{
	template <typename T1, typename T2> 
	using SignalMap = std::unordered_map<T1, T2>;

	using ID_Type = unsigned long long;
public:
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	static EventManager& GetInstance()
	{
		static EventManager instance;
		return instance;
	}

	void ConnectObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string& i_subtopic);
	ScopedConnection ScopedConnectObserver(const ISubject* ip_subject, IObserver* ip_observer,const std::string &i_subtopic);
	bool HasObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string & i_subtopic);
	void DisconnectObserver(const ISubject* ip_subject, IObserver* ip_observer, const std::string &i_subtopic);
	void DisconnectObserver(const ISubject* ip_subject, IObserver* ip_observer);
	void DisconnectObserver(IObserver* ip_observer);
	void RemoveSubject(const ISubject* ip_subject);

	void Notify(const ISubject* ip_source, const std::string & i_subtopic, const Event& i_event);
private:
	EventManager()
		:m_signal_map{}
	{}
	SignalMap<ID_Type, SignalMap<std::string,std::vector<IObserver*>>> m_signal_map; // const ISubject* -> UniqueID, but how can i check if object is alive ?

	std::vector<IObserver*>* FindObserversCollection(ID_Type i_subject_id, const std::string& i_subtopic);
};

