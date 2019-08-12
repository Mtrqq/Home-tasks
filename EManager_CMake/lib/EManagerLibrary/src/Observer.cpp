#include <EManager/Observer.h>

void Observer::OnEvent(const Event& i_event)
{
	std::cout << this << "- observer notified !\n";
}
