#pragma once

#include <memory>

class Event
{

};

typedef std::shared_ptr<Event> EventPtr;

// 동기 큐 사용
template< typename T >
class EventQueue
{

private:
	Concurrency::concurrent_queue<T> m_currencyQueue;

public:
	void Push(const T& t)
	{
		m_currencyQueue.push(t);
	}

	bool Pop(T& t)
	{
		return m_currencyQueue.try_pop(t);
	}
};


class EventListener
{

public:
	virtual void Notify(const EventPtr& ptr)
	{
		m_queue.Push(ptr);
	}

protected:
	EventQueue<EventPtr> m_queue;
};

