#include <EManager/Subject.h>

ISubject::~ISubject()
{
	EventManager::GetInstance().RemoveSubject(this);
}

ISubject::ID_Type ISubject::GetUniqueIdentifier() const
{
	return m_owned_id;
}

ISubject::ID_Type ISubject::GenerateID()
{
	static int unique_id{};
	return unique_id++;
}