clock viewport ball: ball.o viewport.o
	g++ ball.o -o ball -lGL -lGLU -lglut
	g++ viewport.o -o viewport -lGL -lGLU -lglut
	g++ clock.o -o clock -lGL -lGLU -lglut
ball.o viewport.o clock.o: ball.cpp viewport.cpp clock.cpp
	g++ -c ball.cpp viewport.cpp clock.cpp
