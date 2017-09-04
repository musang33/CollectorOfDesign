#pragma once


template< typename T >
class ISingleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

	virtual void Init() = 0;
	virtual void UnInit() = 0;

public:
	virtual ~ISingleton() {};

protected:
	ISingleton() {};

public:
	ISingleton(ISingleton const&) = delete;
	void operator=(ISingleton const&) = delete;
};