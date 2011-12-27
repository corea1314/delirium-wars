#include "Camera.h"

#include "Lair/Lair.h"

#include "glee/GLee.h"

#include <minIni/minIni.h>

#include <algorithm>
#include <string>


///////////////////////////////////////////////////////////////////////////////

Camera::Camera() 
	: m_fZoom(1.0f)
	, m_fAngle(0.0f)
	, m_pEntity(0)
{
}


Camera::~Camera()
{
}

Vector2& Camera::GetPos()
{
	if( m_pEntity )
		return m_pEntity->GetPos();

	return m_vPos;
}

float& Camera::GetAngle()
{
	if( m_pEntity )
		return m_pEntity->GetAngle();

	return m_fAngle;
}


// ============================================================================

CameraMan::CameraMan()
{
	m_pDefaultCamera = new Camera;
	SetActiveCamera(m_pDefaultCamera);
}

CameraMan::~CameraMan()
{
	SAFE_DELETE(m_pDefaultCamera);
}

Camera* CameraMan::Get( const std::string& in_szFilename )
{
	std::map< std::string, Camera* >::iterator it = m_mapCamera.find( in_szFilename );

	if( it != m_mapCamera.end() )
	{
		// found it, return it
//		Lair::GetLogMan()->Log( "SequenceMan", "Loaded sequence from map (%s).", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it
		Camera* pCamera = new Camera;

		unsigned long nTime = Lair::GetSysMan()->GetTime();

		m_mapCamera.insert( std::make_pair(in_szFilename, pCamera ) );
			
		nTime = Lair::GetSysMan()->GetTime() - nTime;
		Lair::GetLogMan()->Log( "CameraMan", "Created camera from file named %s in %d ms.", in_szFilename.c_str(), nTime );
		return pCamera;
	}	
	return 0;
}

