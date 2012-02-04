-------------------------------------------------------------------------------
-- variables: entity logic variables
-------------------------------------------------------------------------------

-- gotoComponent = this:createGotoComponent()
-- turnComponent = this:createTurnComponent()
inputComponent = this:createInputComponent()
visualComponent = this:createVisualComponent()
-- engineComponent = this:createEngineComponent()
physicsComponent = this:createPhysicsComponent()

-------------------------------------------------------------------------------
-- onConnect: called when the entity is connected to the engine
-------------------------------------------------------------------------------
function OnConnect()
	
	visualComponent:play( "sample.spr", false )
	physicsComponent:create_body( "sprite.xbd", true )

end

-------------------------------------------------------------------------------
-- onDisconnect: called when the entity is disconnected from the engine
-------------------------------------------------------------------------------
function OnDisconnect()

	physicsComponent:destroy_body()

end


-------------------------------------------------------------------------------
-- GotoComponent callbacks
-------------------------------------------------------------------------------
-- function OnGotoComponent_DestReached() end
-- function OnGotoComponent_Moving( ratio ) end


-------------------------------------------------------------------------------
-- TurnComponent callbacks
-------------------------------------------------------------------------------
-- function OnTurnComponent_AngleReached() end
-- function OnTurnComponent_Turning( ratio ) end


-------------------------------------------------------------------------------
-- VisualComponent callbacks
-------------------------------------------------------------------------------
-- function OnVisualComponent_EndOfSequence( seqname ) end


-------------------------------------------------------------------------------
-- InputComponent callbacks
-------------------------------------------------------------------------------
-- function OnInputComponent_Keyboard( key ) end
function OnInputComponent_MouseClick( button, screen_x, screen_y, world_x, world_y ) 

	if button == 1 then
		physicsComponent:warp( world_x, world_y, 90 );

	end

end
-- function OnInputComponent_MouseWheel( value ) end


-------------------------------------------------------------------------------
-- EngineComponent callbacks
-------------------------------------------------------------------------------
-- function OnEngineComponent_Update( deltatime ) end

