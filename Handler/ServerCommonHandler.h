#pragma once

/*
Date : 2017-08-30
*/

#include <concurrent_queue.h>

#include <vector>
#include <queue>
#include <mutex>

#include "..\Core\Singleton.h"
#include "..\Core\Event.h"

// 통신에 사용되는 모든 프로토콜은 Event를 상속받는다. 

// Handler 의 인터페이스 클래스이다. 
// 기본적으로 Event를 담을 Queue와
// Queue에 있는 이벤트를 처리할 Dispatcher
// 주기적인 처리를 위한 Update로 구성된다. 
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
	world server 의 경우 종류별로 Handler class 를 만들어서 
	event 및 update를 처리했다. 
	문제는 이렇게 하다보니 Handler마다 event queue 가 나뉘게 되고
	정확하게 serialize 가 되지 않는 현상이 발생했다. 
	이를 개선하기 위해 각각의 class 는 ServerHandlerUnion를 상속받아
	singleton 으로 생성된 Handler 제어 Class 로 정보를 제공하도록 한다.	
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
	std::mutex								m_lock;
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
