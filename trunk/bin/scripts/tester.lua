-------------------------------------------------------------------------------
-- variables: entity logic variables
-------------------------------------------------------------------------------

 gotoComponent = this:createGotoComponent()
 turnComponent = this:createTurnComponent()
 inputComponent = this:createInputComponent()
 visualComponent = this:createVisualComponent()
-- engineComponent = this:createEngineComponent()

 destCounter = 0

-------------------------------------------------------------------------------
-- onConnect: called when the entity is connected to the engine
-------------------------------------------------------------------------------
function OnConnect()
    visualComponent:play( "sample.spr", false )
    gotoComponent:goto( -128, 0, 2 )
end

-------------------------------------------------------------------------------
-- onDisconnect: called when the entity is disconnected from the engine
-------------------------------------------------------------------------------
function OnDisconnect()

end


-------------------------------------------------------------------------------
-- GotoComponent callbacks
-------------------------------------------------------------------------------
function OnGotoComponent_DestReached()

	destCounter = destCounter + 1

	visualComponent:play( "sample.spr", false )

	if destCounter == 0 then
		gotoComponent:goto( -128, 0, 2 )
		turnComponent:turn( -45, 2 )

	elseif destCounter==1 then
		gotoComponent:goto( -128, -128, 2 )
		turnComponent:turn(  45, 2 )

	elseif destCounter==2 then
		gotoComponent:goto( 128, 0, 2 )
		turnComponent:turn( -45, 2 )

	elseif destCounter==3 then
		gotoComponent:goto( 0, 128, 2 )
		turnComponent:turn(  45, 2 )

	else
		gotoComponent:goto( 128, 128, 2 )
		turnComponent:turn( -45, 2 )
		destCounter = -1
	end

end

-- function OnGotoComponent_Moving( ratio ) end


-------------------------------------------------------------------------------
-- TurnComponent callbacks
-------------------------------------------------------------------------------
-- function OnTurnComponent_DestReached() end
-- function OnTurnComponent_Turning( ratio ) end


-------------------------------------------------------------------------------
-- VisualComponent callbacks
-------------------------------------------------------------------------------
-- function OnVisualComponent_EndOfSequence( seqname ) end


-------------------------------------------------------------------------------
-- InputComponent callbacks
-------------------------------------------------------------------------------
function OnInputComponent_Keyboard( key )

	if key == 113 then --113 is letter Q
		visualComponent:play( "sample.spr", false )
		gotoComponent:goto( 0, 0, 2 )
	end
end

--function OnInputComponent_MouseClick( button, state, screen_x, screen_y, world_x, world_y )
--	if button == 0 and state == 1 then
--		gotoComponent:goto( world_x, world_y, 1 )
--	end
--end

function OnInputComponent_MouseWheel( value )

end


-------------------------------------------------------------------------------
-- EngineComponent callbacks
-------------------------------------------------------------------------------
-- function OnEngineComponent_Update( deltatime ) end

