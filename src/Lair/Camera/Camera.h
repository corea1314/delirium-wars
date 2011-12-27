#ifndef _CAMERA_H
#define _CAMERA_H

#include <map>
#include <vector>
#include <string>

#include "Math/Vector2.h"
#include "Engine/Entity.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();
	
	Vector2& GetPos();
	float& GetAngle();
	float& GetZoom() { return m_fZoom; }

	inline CEntity* GetEntity() { return m_pEntity; }
	inline void SetEntity( CEntity* in_pEntity ) { m_pEntity = in_pEntity; }

	const std::string& GetName() { return m_szName; }

private:
	std::string	m_szName;
	float		m_fZoom;
	float		m_fAngle;
	Vector2		m_vPos;
	CEntity*	m_pEntity;
};


class CameraMan
{
public:
	CameraMan();
	virtual ~CameraMan();

	Camera* Get( const std::string& in_szName );

	inline void SetActiveCamera( Camera* in_pCamera ) { if( in_pCamera == 0 ) m_pActiveCamera = m_pDefaultCamera; else m_pActiveCamera = in_pCamera; }
	inline Camera* GetActiveCamera() { return m_pActiveCamera; }
	inline Camera* GetDefaultCamera() { return m_pDefaultCamera; }
		
private:
	Camera*	m_pActiveCamera;
	Camera*	m_pDefaultCamera;

	std::map< std::string, Camera* >	m_mapCamera;
};

#endif//_CAMERA_H