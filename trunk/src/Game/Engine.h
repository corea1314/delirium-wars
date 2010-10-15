#ifndef _ENGINE_H
#define _ENGINE_H

#include "SigSlot.h"

#define NEW_SIGNAL0( name )			\
	private:						\
	signal0<> m_sig##name;			\
	public:							\
		void Connect_##name(has_slots<>* pclass, void (has_slots<>::*pmemfun)()) { m_sig##name.connect( pclass, pmemfun ); }	\
		void Disconnect##name(has_slots<>* pclass) { m_sig##name.disconnect( pclass ); }	\
		void name() { m_sig##name.emit(); }	\
	private:

#define NEW_SIGNAL1( name, arg0 )	\
	private:						\
	signal1<arg0> m_sig##name;		\
	public:							\
	void Connect_##name(has_slots<>* pclass, void (has_slots<>::*pmemfun)(arg0)) { m_sig##name.connect( pclass, pmemfun ); }	\
	void Disconnect##name(has_slots<>* pclass) { m_sig##name.disconnect( pclass ); }	\
	void name( arg0 val0 ) { m_sig##name.emit( val0 ); }	\
	private:

#define NEW_SIGNAL2( name, arg0, arg1 )	\
	private:						\
	signal2<arg0,arg1> m_sig##name;		\
	public:							\
	void Connect_##name(has_slots<>* pclass, void (has_slots<>::*pmemfun)(arg0,arg1)) { m_sig##name.connect( pclass, pmemfun ); }	\
	void Disconnect##name(has_slots<>* pclass) { m_sig##name.disconnect( pclass ); }	\
	void name( arg0 val0, arg1 val1 ) { m_sig##name.emit(val0,val1); }	\
	private:

class CEngine
{
public:	
	NEW_SIGNAL0( Init );
	NEW_SIGNAL0( Exit );
	NEW_SIGNAL1( Update, float );	// delta time
	NEW_SIGNAL0( Render );
};

#endif//_ENGINE_H