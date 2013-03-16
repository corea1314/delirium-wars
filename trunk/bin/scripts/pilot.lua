-------------------------------------------------------------------------------
-- variables: entity logic variables
-------------------------------------------------------------------------------

 gotoComponent = this:createGotoComponent()
 turnComponent = this:createTurnComponent()
 inputComponent = this:createInputComponent()
 visualComponent = this:createVisualComponent()
 asmComponent = this:createAsmComponent()
-- engineComponent = this:createEngineComponent()

 destCounter = 0

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
function OnInputComponent_Keyboard( key, down )

--	if key == 113 then --113 is letter Q
--		asmComponent:set_bool("Jump",true);
--	end
	
	if key == 113 then --113 is letter Q
		asmComponent:set_bool("Throw", down);
	end
end

--function OnInputComponent_MouseClick( button, screen_x, screen_y, world_x, world_y )
--	if button == 0 then
--		gotoComponent:goto( world_x, world_y, 1 )
--	end
--end

function OnInputComponent_MouseWheel( value )

end


-------------------------------------------------------------------------------
-- EngineComponent callbacks
-------------------------------------------------------------------------------
-- function OnEngineComponent_Update( deltatime ) end

