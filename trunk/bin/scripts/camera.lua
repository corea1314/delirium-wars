-------------------------------------------------------------------------------
-- variables: entity logic variables
-------------------------------------------------------------------------------

gotoComponent = this:createGotoComponent()
turnComponent = this:createTurnComponent()
cameraComponent = this:createCameraComponent()
inputComponent = this:createInputComponent()
-- visualComponent = this:createVisualComponent()
-- engineComponent = this:createEngineComponent()

curr_tester = 1;

-------------------------------------------------------------------------------
-- onConnect: called when the entity is connected to the engine
-------------------------------------------------------------------------------
function OnConnect()
    cameraComponent:bind() --bind camera to this entity
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
    if key == 99 then -- 99 is code for character 'c'
        local names = { "tester", "tester2", "tester3" }
        if cameraComponent:bind_to( names[curr_tester] ) == false then
            cameraComponent:bind() -- if cannot bind to entity, then bind to this one instead
        end
        curr_tester = curr_tester + 1
        if curr_tester > 3 then curr_tester = 1 end
    end
end

function OnInputComponent_MouseClick( button, screen_x, screen_y, world_x, world_y ) 
    if button == 0 then
        gotoComponent:goto( world_x, world_y, 1.0 )
    end
end

function OnInputComponent_MouseWheel( value )

	curr_zoom = cameraComponent:get_zoom()

	if value == 1 then
		if curr_zoom > 0.25 then
			cameraComponent:zoom( curr_zoom / 2.0, 1.0 )
		end
	elseif value == -1 then
		if curr_zoom < 4.0 then
			cameraComponent:zoom( curr_zoom * 2.0, 1.0 )
		end
	end
end


-------------------------------------------------------------------------------
-- EngineComponent callbacks
-------------------------------------------------------------------------------
-- function OnEngineComponent_Update( deltatime ) end
