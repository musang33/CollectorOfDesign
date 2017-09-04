#include "ServerCommonHandler.h"

ServerHandlerUnion::ServerHandlerUnion()
{
	theServerUnionHandler::Instance().AppendHandler(this);
}

ServerHandlerUnion::~ServerHandlerUnion()
{
}

void ServerHandlerUnion::Notify(const EventPtr & ptr)
{
	theServerUnionHandler::Instance().Notify(m_indexHandler, ptr);
}

theServerUnionHandler::~theServerUnionHandler()
{
}

int theServerUnionHandler::AppendHandler(ServerHandlerUnion * handler)
{
	m_vecHandler.push_back(handler);

	return m_vecHandler.size();		
}

void theServerUnionHandler::Notify(int indexHandler, const EventPtr & ptr)
{
	stHandlerEvent stEvent(indexHandler, ptr);

	std::scoped_lock lock{ m_lock };
	m_qCurrentPushable.push(ptr);
}

void theServerUnionHandler::Update()
{
	{
		ScopedLook(m_lock);
		std::queue<stHandlerEvent>* tmp = m_qCurrentPushable;
		m_qCurrentPushable = m_qCurrentDispatchable;
		m_qCurrentDispatchable = tmp
	}
		
	while (false == m_qCurrentDispatchable->empty())
	{
		DoDispatch(m_qCurrentDispatchable->front());
		m_qCurrentDispatchable->pop();
	}
	
	for (const auto& i : m_vecHandler)
	{
		i->UpdateInLoop();
	}
}

theServerUnionHandler::theServerUnionHandler()
{
	m_qCurrentPushable = &m_qEvent[0];
	m_qCurrentDispatchable = &m_qEvent[1];

}

bool theServerUnionHandler::DoDispatch(stHandlerEvent & stEvent)
{
	return m_vecHandler[stEvent.indexHandler]->DoDispathc(stEvent.eventPtr);
}
