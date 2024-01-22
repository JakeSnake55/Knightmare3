Building a 3d Game with ImGui and Allegro.

Current features:
	Debug window if defined value is set to true.
	DemoWindow from ImGui
	Primitive drawing
	Neat Structuring (so far)
	Full screen starting window
	Skybox generator from Knightmare 1st version
	Perspectives from Allegro

Work in Progress:
	
	Main Menu system -Somewhat satisfied with worldlist, but selected world info is not yet complete

Planned Features:
	Texture Baker for Sun and Moon textures
	UDP hole punching (or better) multiplayer (means no servers, just connect to friends)
 	Full Linux compatibility
  	Maybe some Mac testing
   	A user friendly installer (maybe a makefile? (though that's only really friendly to programmers))

 Known Issues:
 	Window.cpp uses strcpy_s, which on linux appears unknown? maybe we can use strcpy instead?
	 can be made plaform specific by definining strcpy_s as strcpy if _linux_ is defined
 	Linux version has excessively sensitive camera movement, and mouse is not locked in position during mouse movement. 
	 Solution found, mouse can't be moved whilst visible, hence on linux make sure mouse is invisible when camera can move


Error Codes:
0 : No Issues detected
1 : Could not initialize Allegro
2 : Could not detect ImGui
