#ifndef _FIELD_H
#define _FIELD_H

#include "SigSlot.h"
#include "../gfx.h"

#define FIELD_SIZE_X	1024
#define FIELD_SIZE_Y	512

class CEngine;

class CField : public has_slots<>
{
public:
	CField();

	void Update( float in_fDeltaTime );
	void Render();
	void LoadFile( const char* in_szFilename );

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );

private:
	void RebuildGrid();

private:
	typedef struct  
	{
		unsigned int nDensity;

	} Cell;

	Cell*	m_pCell;

	// Debug grid
	Vertex*		vb_grid;
	int vb_grid_size;
};

#endif//_FIELD_H
