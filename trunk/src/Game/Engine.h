#ifndef _ENGINE_H
#define _ENGINE_H

#include "SigSlot.h"

#define NEW_SIGNAL0( name )			\
	private:						\
	signal0<> m_sig##name;			\
	public:							\
		template<class desttype>	\
		void Connect_##name(desttype* pclass, void (desttype::*pmemfun)()) { m_sig##name.connect( pclass, pmemfun ); }	\
		void Disconnect_##name(has_slots<>* pclass) { m_sig##name.disconnect( pclass ); }	\
		void name() { m_sig##name.emit(); }	\
	private:

#define NEW_SIGNAL1( name, arg0 )	\
	private:						\
	signal1<arg0> m_sig##name;		\
	public:							\
	template<class desttype>	\
	void Connect_##name(desttype* pclass, void (desttype::*pmemfun)(arg0)) { m_sig##name.connect( pclass, pmemfun ); }	\
	void Disconnect_##name(has_slots<>* pclass) { m_sig##name.disconnect( pclass ); }	\
	void name( arg0 val0 ) { m_sig##name.emit( val0 ); }	\
	private:

#define NEW_SIGNAL2( name, arg0, arg1 )	\
	private:						\
	signal2<arg0,arg1> m_sig##name;		\
	public:							\
	template<class desttype>	\
	void Connect_##name(desttype* pclass, void (desttype::*pmemfun)(arg0,arg1)) { m_sig##name.connect( pclass, pmemfun ); }	\
	void Disconnect_##name(has_slots<>* pclass) { m_sig##name.disconnect( pclass ); }	\
	void name( arg0 val0, arg1 val1 ) { m_sig##name.emit(val0,val1); }	\
	private:


class CField;
class CClock;
class CCamera;

class CEngine
{
public:	
	CEngine();
	virtual ~CEngine();

	const CClock*	GetClock() const { return m_pClock; }
	CCamera*		GetCamera() const { return m_pCamera; }
	CField*			GetField() const { return m_pField; }

	NEW_SIGNAL1( Update, float );	// delta time
	NEW_SIGNAL0( Render );
	NEW_SIGNAL0( RenderGUI );

	NEW_SIGNAL1( LoadFile, const char* ); // filename
	
private:
	CField*		m_pField;
	CClock*		m_pClock;
	CCamera*	m_pCamera;

	//todo: add entity factories here
};

#endif//_ENGINE_H