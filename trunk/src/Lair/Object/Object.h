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

#endif//_OBJECT_H
