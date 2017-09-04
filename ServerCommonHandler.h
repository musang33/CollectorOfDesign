#pragma once

/*
Date : 2017-08-30
*/

#include <concurrent_queue.h>
#include <memory>
#include <vector>
#include <queue>

#include ""

// ��ſ� ���Ǵ� ��� ���������� Event�� ��ӹ޴´�. 
class Event
{

};

typedef std::shared_ptr<Event> EventPtr;

// ���� ť ���
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
	void Notify(const EventPtr& ptr)
	{
		m_queue.Push(ptr);
	}

protected:
	EventQueue<EventPtr> m_queue;
};


// Handler �� �������̽� Ŭ�����̴�. 
// �⺻������ Event�� ���� Queue��
// Queue�� �ִ� �̺�Ʈ�� ó���� Dispatcher
// �ֱ����� ó���� ���� Update�� �����ȴ�. 
class ServerHandler : public EventListener
{
public:
	bool Initialize()
	{
		initialize();
	}

	void Update()
	{
		EventPtr ptr;
		while (m_queue.Pop(ptr))
		{
			dispatch(ptr);
		}

		update();
	}


	void Finish()
	{
		finish();
	}

protected:
	virtual bool initialize() = 0;
	virtual void update() = 0;
	virtual void finish() = 0;
	virtual void dispatch(EventPtr& ptr) = 0;
};

/*
	world server �� ��� �������� Handler class �� ���� 
	event �� update�� ó���ߴ�. 
	������ �̷��� �ϴٺ��� Handler���� event queue �� ������ �Ǹ鼭
	��Ȯ�ϰ� serialize �� ���� �ʴ� ������ �߻��ߴ�. 
	�̸� �����ϱ� ���� ������ class �� ServerHandlerUnion�� ��ӹ޾�
	singleton ���� ������ Handler ���� Class �� ������ �����ϵ��� �Ѵ�.	
*/

class ServerHandlerUnion : public ServerHandler
{
protected:
	int m_indexHandler;

public:
	ServerHandlerUnion();
	virtual ~ServerHandlerUnion();

	virtual void Notify(const EventPtr& ptr) override;
	virtual bool DoDispathc(EventPtr& ptr) = 0;
	int GetIndexHandler()
	{
		return m_indexHandler;
	}

	void UpdateInLoop()
	{
		update();
	}
};


template <typename T>
class ServerCommonHandler : public ServerHandlerUnion
{
public:
	typedef void (T::*Func)(EventPtr& ptr);
	typedef T ThisClass;

protected:
	Dispatcher<T> m_dispatcher;

public:
	ServerCommonHandler()
		: ServerHandlerUnion()
		, m_dispatcher(static_cast<T>(this))
	{}

	virtual ~ServerCommonHandler() override {}
	virtual void dispatch(EventPtr& ptr) override { m_dispatcher.Dispatch(ptr); }
	virtual bool DoDispatch(EventPtr& ptr) override { return m_dispatcher.Dispatch(ptr); }
};



class theServerUnionHandler : public ISingleton <theServerUnionHandler>
{
	friend ISingleton <theServerUnionHandler>;

private:
	struct stHandlerEvent
	{
		int			indexHandler;
		EventPtr	eventPtr;
		stHandlerEvent(int i, const EventPtr &ptr)
			: indexHandler(i)
			, eventPtr(ptr)
		{};
	};

	std::vector<ServerHandlerUnion*>		m_vecHandler;
	Mutex									m_lock;
	std::queue<stHandlerEvent>				m_qEvent[2];
	std::queue<stHandlerEvent>*				m_qCurrentPushable;
	std::queue<stHandlerEvent>*				m_qCurrentDispatchable;

public:
	virtual ~theServerUnionHandler() override;
	int AppendHandler(ServerHandlerUnion* handler);
	void Notify(int indexHandler, const EventPtr& ptr);
	void Update();

private:
	theServerUnionHandler();
	bool DoDispatch(stHandlerEvent& stEvent);
};
