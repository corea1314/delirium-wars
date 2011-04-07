#ifndef _DATABASE_H
#define _DATABASE_H

#include <map>
#include <string>

class Object;

typedef void (Object::*ObjectMethod)(); 

class Data 
{
public:
	virtual bool Set( int i ) { return false; }
	virtual bool Get( int& i ) { return false; }

	virtual bool Set( float f ) { return false; }
	virtual bool Get( float& f ) { return false; }

	virtual bool Set( const std::string& s ) { return false; }
	virtual bool Get( std::string& s ) { return false; }

	virtual bool Set( Object* o ) { return false; }
	virtual bool Get( Object*& o ) { return false; }

	virtual bool Set( ObjectMethod m ) { return false; }
	virtual bool Get( ObjectMethod& m ) { return false; }
		
	static Data* Create( int i );
	static Data* Create( float f );
	static Data* Create( std::string& s );
	static Data* Create( Object* o );
	static Data* Create( ObjectMethod m );
};

class DataInt : public Data 
{
public:
	DataInt( int in_nValue = 0 ) : m_nValue(in_nValue) {}

	virtual bool Set( int i ) { m_nValue = i; return true; }
	virtual bool Get( int& i ) { i = m_nValue; return true; }

	virtual bool Set( float f ) { m_nValue = (int)f; return true; }
	virtual bool Get( float& f ) { f = (float)m_nValue; return true; }

	virtual bool Set( const std::string& s ) { m_nValue = atoi( s.c_str() ); return true; }
	virtual bool Get( std::string& s ) { char t[16]; s = _itoa_s(m_nValue,t,10); return true; }

private:
	int	m_nValue;
};

class DataFloat : public Data 
{
public:
	DataFloat( float in_fValue = 0 ) : m_fValue(in_fValue) {}

	virtual bool Set( int i ) { m_fValue = (float)i; return true; }
	virtual bool Get( int& i ) { i = (int)m_fValue; return true; }

	virtual bool Set( float f ) { m_fValue = f; return true; }
	virtual bool Get( float& f ) { f = m_fValue; return true; }

	virtual bool Set( const std::string& s ) { m_fValue = (float)atof( s.c_str() ); return true; }
	virtual bool Get( std::string& s ) { char t[32]; sprintf_s( t, "%f", m_fValue ); s = t; return true; }

private:
	float	m_fValue;
};

class DataString : public Data 
{
public:
	DataString( const std::string& in_strValue ) : m_strValue(in_strValue) {}

	virtual bool Set( int i ) { char t[16]; m_strValue = _itoa_s(i,t,10); return true; }
	virtual bool Get( int& i ) { i = atoi(m_strValue.c_str()); return true; }

	virtual bool Set( float f ) { char t[32]; sprintf_s( t, "%f", f ); m_strValue = t; return true; }
	virtual bool Get( float& f ) { f = (float)atof( m_strValue.c_str() ); return true; }

	virtual bool Set( const std::string& s ) { m_strValue = s; return true; }
	virtual bool Get( std::string& s ) { s = m_strValue; return true; }

private:
	std::string	m_strValue;
};

class DataObject : public Data 
{
public:
	DataObject( Object*	in_pObject ) : m_pObject(in_pObject) {}

	virtual bool Set(  Object* o )	{ m_pObject = o; return true; }
	virtual bool Get( Object*& o )	{ o = m_pObject; return true; }

private:
	Object*	m_pObject;
};

class DataObjectMethod : public Data 
{
public:
	DataObjectMethod( ObjectMethod in_Method ) : m_Method(in_Method) {}

	virtual bool Set(  ObjectMethod m )	{ m_Method = m; return true; }
	virtual bool Get( ObjectMethod& m )	{ m = m_Method; return true; }

private:
	ObjectMethod	m_Method;
};

class Database
{
public:
	template <typename T> bool Set( const std::string& s, T t )	 { if( m_pTempData = GetData(s) ) { return m_pTempData->Set(t);	 } return false; }
	template <typename T> bool Get( const std::string& s, T& t ) { if( m_pTempData = GetData(s) ) { return m_pTempData->Get(t);	 } return false; }

	template <typename T>
	bool	Register( const std::string& s, T t )
	{
		data_map_it it = m_mapData.find(s); 
		if( it == m_mapData.end() ) 
		{
			m_mapData.insert( std::make_pair(s,Data::Create(t)));
			return true;
		}
		return false;
	}

	Data*	GetData( const std::string& s ) 
	{ 
		data_map_it it = m_mapData.find(s); 
		if( it != m_mapData.end() ) 
		{
			return it->second;
		}
		return 0;
	}

private:
	typedef std::map<std::string, Data*> data_map;
	typedef data_map::iterator data_map_it;
	data_map	m_mapData;
	Data*		m_pTempData;
};

#endif//_DATABASE_H
