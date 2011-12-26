#pragma once

/*
template <class T>
class Delegate
{
public:
	typedef void (T::*Function)();

	Delegate( T* inInstance, Function inFunction ) : mpInstance(inInstance), mpFunction(inFunction) {}
	void operator()() { (mpInstance->*mpFunction)(); }

private:
	T* mpInstance;
	Function mpFunction;
};
*/


template <class T, class arg0 >
class Delegate1
{
public:
	typedef void (T::*Function)( arg0 inArg0 );

	Delegate1( T* inInstance, Function inFunction, arg0 inArg0 ) : mpInstance(inInstance), mpFunction(inFunction), mArg0(inArg0) {}
	
	void operator()() 
	{ 
		(mpInstance->*mpFunction)( mArg0 ); 
	}

private:
	arg0		mArg0;
	T*			mpInstance;
	Function	mpFunction;
};



// TODO add more with args