#ifndef _ANALOG_H
#define _ANALOG_H

class TiXmlElement;

#include <map>
#include <vector>
#include <string>

class AnalogInstance
{
public:
	virtual const char* GetType() const = 0;
	virtual void Load( TiXmlElement* in_pxmlInstance ) = 0;

	void ParseProperties( TiXmlElement* in_pxmlInstance );

	std::map<std::string,std::string> m_mapProperties;

	float z;
};

class AnalogInstancePoint : public AnalogInstance
{
public:
	virtual const char* GetType() const { return "Point"; }
	virtual void Load( TiXmlElement* in_pxmlInstance );

	float x,y;
};

class AnalogInstanceRect : public AnalogInstance
{
public:
	virtual const char* GetType() const { return "Rect"; }
	virtual void Load( TiXmlElement* in_pxmlInstance );

	float x,y;
	float w,h;
};

class AnalogInstancePoly : public AnalogInstance
{
public:
	virtual const char* GetType() const { return "Poly"; }
	virtual void Load( TiXmlElement* in_pxmlInstance );

	class Point { public: float x,y; };

	std::vector<Point>	m_vecPoint;
};

class AnalogMap
{
public:
	std::vector<AnalogInstance*>	m_vecInstance;

	void Load( std::string in_szName );
};

/*
Entity ideas

fader

Commands idea
- parenting and hierarchy changes
- 

Messaging
- On Event X, call Function Y from Target in Z secs with W parameter


For each entity we need to determine list of Inputs and Outputs
Inputs are functions which we can fire
Outputs are events which can be fire


*/


#endif//_ANALOG_H