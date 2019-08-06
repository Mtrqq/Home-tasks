#pragma once

#include <string>

class ISubject;
class IObserver;

class ScopedConnection
{
public:
	ScopedConnection(const ISubject* ip_subject, const std::string& i_subtopic, IObserver* ip_observer)
		:mp_subject{ip_subject}
		,m_subtopic{i_subtopic}
		,mp_observer{ip_observer}
	{}
	~ScopedConnection()
	{
		Disconnect();
	}
	void Disconnect();
	bool IsConnected() const;
private:
	const ISubject* mp_subject;
	std::string m_subtopic;
	IObserver* mp_observer;
};

