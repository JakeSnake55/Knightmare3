Building a 3d Game with ImGui and Allegro.

Current features:
	Debug window if defined value is set to true.
	DemoWindow from ImGui
	Primitive drawing
	Neat Structuring (so far)
	Full screen starting window

Work in Progress:
	Perspectives from Allegro
	Main Menu system

Planned Features:
	Skybox generator from Knightmare 1st version
	Texture Baker for Sun and Moon textures
 	Full Linux compatibility
  	Maybe some Mac testing
   	A user friendly installer (maybe a makefile? (though that's only really friendly to programmers))

 Known Issues:
 	Window.cpp uses strcpy_s, which on linux appears unknown? maybe we can use strcpy instead?
 	Linux version has excessively sensitive camera movement, and mouse is not locked in position during mouse movement.
