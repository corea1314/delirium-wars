--- modules

--- create any variables structure required by the logic here
--- component creation routines

gotoComponent = this:createGotoComponent();

visualComponent = this:createVisualComponent();


-- init
function init()
	gotoComponent:goto( 0, 0, 5 );
	visualComponent:play( "sample.spr", true );
end

--- component callbacks

--- goto
function OnGotoComponent_DestReached()

end

--- visual ---

--- engine callbacks ---


