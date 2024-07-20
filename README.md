Building a 3d Game with ImGui and Allegro.

Current features:
	Debug window if defined value is set to true.
	DemoWindow from ImGui
	Primitive drawing
	Neat Structuring (so far)
	Full screen starting window
	Skybox generator from Knightmare 1st version
	Perspectives from Allegro
	Loading from incorrect folder will run without assets
	Set up project to now place executable in expected location on compile

Work in Progress:
	Debug data
	Main Menu system -Somewhat satisfied with worldlist, but selected world info is not yet complete

Planned Features:
	Texture Baker for Sun and Moon textures
	UDP hole punching (or better) multiplayer (means no servers, just connect to friends)
 	Full Linux compatibility
  	Maybe some Mac testing
  A user friendly installer (maybe a makefile? (though that's only really friendly to programmers))

Known Issues:
	Linux version has strong flashes while skybox is drawn, Windows has flashing on transparent bits of ImGui's transparency.
	Linux shows build_chunks() leads to a segmentation fault.
	Linux shows something in removing the main menu to enter a map leads to a segmentation fault of some kind.
	ValGrind makes me worried about memory leaks and possible issues. I want to root out all warnings, and possible issues.
	Game runs really slowly, most of the time is spent on Processing, but disabling keyboard, display and mouse do not affect the time taken.


Error Codes:
0 : No Issues detected
1 : Could not initialize Allegro
2 : Could not detect ImGui
