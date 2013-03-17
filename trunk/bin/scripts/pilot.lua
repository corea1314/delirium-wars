-------------------------------------------------------------------------------
-- variables: entity logic variables
-------------------------------------------------------------------------------

-- gotoComponent = this:createGotoComponent()
-- turnComponent = this:createTurnComponent()
-- inputComponent = this:createInputComponent()
-- visualComponent = this:createVisualComponent()
 asmComponent = this:createAsmComponent()
 engineComponent = this:createEngineComponent()
 gamepadComponent = this:createGamepadComponent()


-------------------------------------------------------------------------------
-- onConnect: called when the entity is connected to the engine
-------------------------------------------------------------------------------
function OnConnect()
    asmComponent:load("pilot.asm")
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
-- function OnInputComponent_Keyboard( key, down ) end
-- function OnInputComponent_MouseClick( button, screen_x, screen_y, world_x, world_y ) end
-- function OnInputComponent_MouseWheel( value ) end

-------------------------------------------------------------------------------
-- EngineComponent callbacks
-------------------------------------------------------------------------------
function OnEngineComponent_Update( deltatime ) 

	asmComponent:set_bool("Throw", gamepadComponent:is_button_just_down(0,0) )

	if gamepadComponent:is_pad_down(0,1) then
		asmComponent:set_float( "Speed", 1.0 )
	else
		asmComponent:set_float( "Speed", 0.0 )
	end

end

