-------------------------------------------------------------------------------
-- variables: entity logic variables
-------------------------------------------------------------------------------

-- gotoComponent = this:createGotoComponent()
-- turnComponent = this:createTurnComponent()
 inputComponent = this:createInputComponent()
-- visualComponent = this:createVisualComponent()
 engineComponent = this:createEngineComponent()

-------------------------------------------------------------------------------
-- onConnect: called when the entity is connected to the engine
-------------------------------------------------------------------------------
function OnConnect()
	engineComponent:create_entity( "camera", "scripts/camera.lua", 0,0,0 )
	engineComponent:create_entity( "tester", "scripts/tester.lua", 0,0,0 )
end

-------------------------------------------------------------------------------
-- onDisconnect: called when the entity is disconnected from the engine
-------------------------------------------------------------------------------
function OnDisconnect()

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
function OnInputComponent_Keyboard( key ) 

end

function OnInputComponent_MouseClick( button, screenx, screeny, worldx, worldy )

	if button == 1 then
        engineComponent:create_entity( "tester2", "scripts/tester.lua", worldx, worldy, 0 )
    end
end

function OnInputComponent_MouseWheel( value ) 
	engineComponent:create_entity( "tester3", "scripts/tester.lua", value * 100, 0, 0 )
end


-------------------------------------------------------------------------------
-- EngineComponent callbacks
-------------------------------------------------------------------------------
-- function OnEngineComponent_Update( deltatime ) end



