
#ifndef _CONTEXT_H
#define _CONTEXT_H

class IContext
{
public:
	virtual void OnKey( unsigned char key );
	virtual void OnMouseClick( int button, int state, int x, int y );
	virtual void OnMouseMove( int x, int y );

	virtual void OnDisplay();
	virtual void OnReshape( int w, int h );
	virtual void OnUpdate( float t, float dt );
	
	virtual bool Save( FILE* ) = 0;
	virtual bool Load( FILE* ) = 0;
};


#endif//_CONTEXT_H
