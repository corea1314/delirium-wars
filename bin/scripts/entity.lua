--- modules

--- create any variables structure required by the logic here
--- component creation routines

destCounter = 0

gotoComponent = this:createGotoComponent();
turnComponent = this:createTurnComponent();
visualComponent = this:createVisualComponent();


-- init
function init()
	gotoComponent:goto( 128, 128, 2 );
	visualComponent:play( "sample.spr", false );
end

--- component callbacks

--- goto
function OnGotoComponent_DestReached()

	destCounter = destCounter + 1

	visualComponent:play( "sample.spr", false );

	if destCounter == 0 then
		gotoComponent:goto( -128, 0, 2 );
		turnComponent:turn( -90, 2 );
	elseif destCounter==1 then
		gotoComponent:goto( -128, -128, 2 );
		turnComponent:turn(  90, 2 );
	elseif destCounter==2 then
		gotoComponent:goto( 128, 0, 2 );
	elseif destCounter==3 then
		gotoComponent:goto( 0, 128, 2 );
		turnComponent:turn( -90, 2 );
	else
		gotoComponent:goto( 128, 128, 2 );
		turnComponent:turn(  90, 2 );
		destCounter = 0
	end

end

function OnGotoComponent_Moving( ratio )

end

--- visual ---

--- engine callbacks ---


