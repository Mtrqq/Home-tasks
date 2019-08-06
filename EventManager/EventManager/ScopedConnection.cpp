#include "pch.h"
#include "ScopedConnection.h"
#include "EventManager.h"

void ScopedConnection::Disconnect()
{
	EventManager::GetInstance().DisconnectObserver(mp_subject, mp_observer, m_subtopic);
}

bool ScopedConnection::IsConnected() const
{
	return EventManager::GetInstance().HasObserver(mp_subject, mp_observer, m_subtopic);
}
