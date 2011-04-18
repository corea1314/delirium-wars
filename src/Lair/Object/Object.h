#ifndef _OBJECT_H
#define _OBJECT_H

#include "../Database/Database.h"

class Object
{
public:
	bool CreateDataBaseFromFile( const std::string& in_szFilename );

	template <typename T> bool Set( const std::string& s, T t )	{ return m_DB.Set(t); }
	template <typename T> bool Get( const std::string& s, T& t )	{ return m_DB.Get(t); }
	template <typename T> bool Register( const std::string& s, T t )	{ return m_DB.Register(s,t);	}
		
	void Call( const std::string& s ) 
	{
		ObjectMethod m;
		if( m_DB.Get( s, m ) && m != NULL )
		{
			(this->*m)();
		}
	}
	
private:
	Data*	GetData( const std::string& s ) { return m_DB.GetData(s); }

private:
	Database	m_DB;
};

template <class T>
class ObjectPool
{
public:
	void Reserve( int in_nCount )
	{
		m_lstFree.push_back( new T );
	}
	void New( T** out_ppObject )
	{
		if( m_lstFree.back() == m_lstFree.end() )
			m_lstFree.push_back( new T );

		*out_ppObject = m_lstFree.front();
		m_lstFree.pop_front();

		(*out_ppObject)->Connect();
	}
	void Delete( T** out_ppObject )
	{
		(*out_ppObject)->Disconnect();

		m_lstFree.push_front( *out_ppObject );
		*out_ppObject = 0;
	}

private:
	std::list<T*>	m_lstFree;
};

class ObjectFactory
{
public:
	ObjectFactory() : m_pEngine {}

	bool Register( const std::string& in_szName,  );
	Object* Create( const std::string& in_szName );

private:
	std::map< std::string, ObjectCreator* >	m_mapObjectCreator;
};

#endif//_OBJECT_H
