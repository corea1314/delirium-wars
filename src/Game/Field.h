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

    Vector2 GetPosition() const { return m_v2Position; }
    void SetPosition(const Vector2 & val) { m_v2Position = val; }

    unsigned int GetGridSizeX() const { return FIELD_SIZE_X; }
    unsigned int GetGridSizeY() const { return FIELD_SIZE_Y; }

    Vector2 GetCellWorldSize() const { return m_v2CellWorldSize; }
    void SetCellWorldSize( const Vector2 & val);
    Vector2 GetGridWorldSize() const { return m_v2GridWorldSize; }

    //Index will 0 if world point is out of bound. Return true when point is in bound.
    bool WorldPositionToIndex( const Vector2 & in_v2WorldPosition, unsigned int & out_IndexX, unsigned int & out_IndexY) const;
    bool WorldPositionToIndexHorizontal( float in_fXWorldPosition, unsigned int & out_IndexX) const;
    bool WorldPositionToIndexVertical( float in_fYWorldPosition, unsigned int & out_IndexY) const;
    
    //World position will be 0.0f if index are out of bound. Return true if index are in bound.
    bool IndexToWorldPosition( unsigned int in_IndexX, unsigned int in_IndexY, Vector2& out_v2WorldPosition) const;
    bool IndexToWorldPositionHorizontal( unsigned int in_IndexX, float& out_fXWorldPosition ) const;
    bool IndexToWorldPositionVertical( unsigned int in_IndexY, float& out_fYWorldPosition ) const;


private:
	void RebuildGrid();

private:

    //How big is a cell in world coordinate
    Vector2 m_v2CellWorldSize;
    Vector2 m_v2Position;
    //Optimization
    Vector2 m_v2InverseCellWorldSize;
    Vector2 m_v2GridWorldSize;
        
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
