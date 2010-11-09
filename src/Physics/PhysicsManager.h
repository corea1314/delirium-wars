#ifndef _PHYSICS_MANAGER_H__
#define _PHYSICS_MANAGER_H__

#include <vector>
#include "../Game/SigSlot.h"

class CField;
class CPhysicsBody;
class CEngineProxy;
//This is mean to be the physics engine. It will implement the main update loop and will broadcast physics signal so the syncher an collision listener can operate.
class CPhysicsManager
{
	private :
		float m_fTimeResolution;
		int m_nMaxSubStep;
		float m_fTimeAccumulator;

        CField* m_pGrid;

        CEngineProxy* m_pEngineProxy;

		//We choose vector because it is easier to iterate over it and pushing new elements is very fast
		std::vector<CPhysicsBody*> m_vPhysicsBody;
		std::vector<CPhysicsBody*> m_vActivePhysicsBody;
		//std::vector<CCollisionBodyOnBodyResult*> m_vBodyOnBody;

		void PhysicsSubStep( float in_fDeltaTime , float in_fInterpolationRatio);
		void ApplyPhysics( float in_fDeltaTime, float in_fInterpolationRatio);
		void HandleCollisionBodyOnBody();
        void HandleCollisionBodyOnGrid();

	public :
		CPhysicsManager( CEngineProxy* in_pEngineProxy, float in_fTimeResolution = 0.1666 , int in_mMaximumSubStep = 0 );
		~CPhysicsManager();

		void SetMaximumSubStep( int in_nMaxSubStep ) { m_nMaxSubStep = in_nMaxSubStep = 10; }
		int GetMaximumSubStep() const { return m_nMaxSubStep; }

		void SetTimeResolution( float in_fTimeResolution) { m_fTimeResolution = in_fTimeResolution; }
		float GetTimeResolution() const { return m_fTimeResolution; }

		//Return the amount of substep accomplished
		int Update( float in_fDeltaTime );

		//Body creation/getter/destruction methods
		//Create
		//Object are always in the world but are not activated by default. Call Activate(true) on a body to activate it.
		CPhysicsBody* CreatePhysicsBody(const char * in_szName);
		int GetPhysicsBodyCount() const { return m_vPhysicsBody.size(); }
		//Allowas you to iterate over all bodies
		CPhysicsBody* GetPhysicsBody( int in_nIndex ) const;

		//Destroy
		void DestroyPhysicsbody( CPhysicsBody* in_pPhysicsbody );
		void DestroyAllBody();

        CField* GetGrid() const { return m_pGrid; }
        void SetGrid(CField* val) { m_pGrid = val; }
};




#endif //_PHYSICS_MANAGER_H__