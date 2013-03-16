
#include "Lair/Lair.h"

#include "../sdk/tinyxml/tinyxml.h"

#include "ASM.h"

namespace ASM
{
	CLayer::CLayer()
	{
		mpSprite = Lair::GetSpriteMan()->GetSprite();
	}

	CLayer::~CLayer()
	{
		if( mpSprite )
		{
			Lair::GetSpriteMan()->FreeSprite( mpSprite );
			mpSprite = 0;
		}
	}

	void CLayer::Reset()
	{
		Transit( mInitialStateName );
	}

	void CLayer::Transit( const std::string& inTransitStateName )
	{
		mCurrentStateName = inTransitStateName;
		mpCurrentState = (mStates.find(mCurrentStateName)->second);

		mpSprite->Play( mpCurrentState->mSequenceName, mpCurrentState->mLoop );
	}

	void CLayer::Update( float in_fDeltaTime )
	{			
		mTransitStateName = mpCurrentState->EvaluateTransitStateName();
		if( !mTransitStateName.empty() && mTransitStateName != mCurrentStateName )
			Transit( mTransitStateName );

		mpSprite->Update(in_fDeltaTime);
	}

	void CMachine::Set( float x, float y, float a, float sx, float sy )
	{
		std::for_each( mLayers.begin(), mLayers.end(), [x,y,a,sx,sy]( layer_ptr Layer ) { Layer->mpSprite->Set(x,y,a,sx,sy); } );
	}

	void CMachine::Reset()
	{
		std::for_each( mLayers.begin(), mLayers.end(), []( layer_ptr Layer ) { Layer->Reset(); } );
	}

	void CMachine::Update( float in_fDeltaTime )
	{
		std::for_each( mLayers.begin(), mLayers.end(), [in_fDeltaTime]( layer_ptr Layer ) { Layer->Update(in_fDeltaTime); } );
	}

	void CMachine::Load( const char* inFilename )
	{
		TiXmlDocument doc(inFilename);
		if (!doc.LoadFile())
			return;

		TiXmlElement* inXmlASM = doc.FirstChildElement("ASM");

		// Parameters
		ParseParameters( inXmlASM->FirstChildElement("Parameters") );

		// Layers -> States -> Transitions -> Conditions
		ParseLayers( inXmlASM->FirstChildElement("Layers") );

		Reset();
	}
	
	bool CMachine::FindParameter( const std::string& inName, parameter_ptr& outParameter )
	{
		std::map< std::string, parameter_ptr >::iterator It = mParameters.find(inName);
		if( It != mParameters.end() )
		{
			outParameter = It->second;
			return true;
		}
		return false;
	}

	void CMachine::ParseParameters( TiXmlElement* pxmlParameters )
	{
		if( pxmlParameters )
		{
			for( TiXmlElement* pxmlParameter = pxmlParameters->FirstChildElement("Parameter"); pxmlParameter; pxmlParameter = pxmlParameter->NextSiblingElement("Parameter") )
			{
				const char* Name = pxmlParameter->FirstChildElement("Name")->GetText();
				const char* Type = pxmlParameter->FirstChildElement("Type")->GetText();

				if( _stricmp(Type,"Bool") == 0 )
				{
					bool V = false;
					parameter_ptr Parameter(new CParameter(V));
					mParameters.insert( std::make_pair(Name, Parameter) );
				}
				else if( _stricmp(Type,"Float") == 0 )
				{
					float V = 0;
					parameter_ptr Parameter(new CParameter(V));
					mParameters.insert( std::make_pair(Name, Parameter) );
				}
				else if( _stricmp(Type,"Integer") == 0 )
				{
					int V = 0;
					parameter_ptr Parameter(new CParameter(V));
					mParameters.insert( std::make_pair(Name, Parameter) );
				}				
			}
		}
	}

	void CMachine::ParseConditions( TiXmlElement* pxmlConditions, layer_ptr& Layer, state_ptr& State, transition_ptr& Transition )
	{
		if( pxmlConditions )
		{
			for( TiXmlElement* pxmlCondition = pxmlConditions->FirstChildElement(); pxmlCondition; pxmlCondition = pxmlCondition->NextSiblingElement() )
			{
				if( _stricmp( pxmlCondition->Value(), "ConditionParameter" ) == 0 )
				{									
					CCondition::Operator Op = CCondition::StringToOperator( pxmlCondition->FirstChildElement("Operator")->GetText() );
					const char* Value = pxmlCondition->FirstChildElement("Value")->GetText();

					parameter_ptr Parameter = mParameters.find( pxmlCondition->FirstChildElement("Parameter")->GetText() )->second;

					switch( Parameter->GetType() )
					{
					case CParameter::Integer:
						{
							condition_ptr Condition(new CConditionParameter<int>((int*)Parameter->GetValue(), Op, atoi(Value) ) );
							Transition->mConditions.push_back(Condition);
						}
						break;
					case CParameter::Bool:
						{
							condition_ptr Condition(new CConditionParameter<bool>((bool*)Parameter->GetValue(), Op, _stricmp(Value,"True")==0 ) );
							Transition->mConditions.push_back(Condition);
						}
						break;
					case CParameter::Float:
						{
							condition_ptr Condition(new CConditionParameter<float>((float*)Parameter->GetValue(), Op, (float)atof(Value) ) );
							Transition->mConditions.push_back(Condition);
						}
						break;
					}
				}
				else if( _stricmp( pxmlCondition->Value(), "ConditionSequenceEnd" ) == 0 )
				{
					condition_ptr Condition(new CConditionSequenceEnd(Layer->mpSprite) );
					Transition->mConditions.push_back(Condition);
				}
			}
		}
	}

	void CMachine::ParseTransitions( TiXmlElement* pxmlTransitions, layer_ptr& Layer, state_ptr& State )
	{
		if( pxmlTransitions )
		{
			for( TiXmlElement* pxmlTransition = pxmlTransitions->FirstChildElement("Transition"); pxmlTransition; pxmlTransition = pxmlTransition->NextSiblingElement("Transition") )
			{
				transition_ptr Transition(new CTransition);

				Transition->mTransitStateName = pxmlTransition->FirstChildElement("TransitStateName")->GetText();

				ParseConditions( pxmlTransition->FirstChildElement("Conditions"), Layer, State, Transition );

				State->mTransitions.push_back(Transition);
			}
		}
	}

	void CMachine::ParseStates( TiXmlElement* pxmlStates, layer_ptr& Layer )
	{
		if( pxmlStates )
		{
			for( TiXmlElement* pxmlState = pxmlStates->FirstChildElement("State"); pxmlState; pxmlState = pxmlState->NextSiblingElement("State") )
			{
				state_ptr State(new CState);

				State->mName = pxmlState->FirstChildElement("Name")->GetText();
				State->mSequenceName = pxmlState->FirstChildElement("SequenceName")->GetText();

				if( TiXmlElement* pxmlLoop = pxmlState->FirstChildElement("Loop") )
					State->mLoop = (_stricmp(pxmlLoop->GetText(),"true") == 0);
				else
					State->mLoop = false;

				ParseTransitions( pxmlState->FirstChildElement("Transitions"), Layer, State );

				Layer->mStates.insert( std::make_pair(State->mName, State) );
			}				
		}
	}

	void CMachine::ParseLayers( TiXmlElement* pxmlLayers )
	{
		if( pxmlLayers )
		{
			for( TiXmlElement* pxmlLayer = pxmlLayers->FirstChildElement("Layer"); pxmlLayer; pxmlLayer = pxmlLayer->NextSiblingElement("Layer") )
			{
				layer_ptr Layer(new CLayer);
				Layer->mName = pxmlLayer->FirstChildElement("Name")->GetText();
				Layer->mInitialStateName = pxmlLayer->FirstChildElement("InitialStateName")->GetText();

				ParseStates( pxmlLayer->FirstChildElement("States"), Layer );

				mLayers.push_back(Layer);
			}
		}
	}
}