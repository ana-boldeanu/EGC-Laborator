// Boldeanu Ana-Maria
// EGC - Homework 2 - Car Race


================================ Description ================================

	This game is a simplified version of a car race, a third-person 3D 
where the player drives a car along a race track, trying to reach the finish
line within good time, while also having to dodge a few obstacles on the way.

	* The player controls a spaceship! (Yeah, I used a spaceship model
instead of a normal car);
	* The beginning of the race course is marked by an arc-like structure;
	* The player can control the vehicle using WASD keys (the vehicle can
move forward/backward and turn left/right);
	* The spaceship should not leave the course (if this happens, it is not
allowed to advance);
	* There are obstacles (UFOs) that have a predefined route on the
course;
	* The player's spaceship should not get hit by these UFOs (if this
happens, the spaceship is stopped for a few moments);
	* The race track is continuous and has the shape of a star;
	* Decorative purple trees and the curvature of the horizontal line
make up a beautiful landscape;
	* There is a minimap in the bottom right corner of the window, used
to locate the player's spaceship in the nearby area.
	

================================ How to play ================================

	* (dev step) Make sure that src/main.cpp runs the Race program
i.e. World* world = new m1::Race();
	* Run the program;
	* Press W/S to move forward/backward and A/D to turn left/right;
	* Use the minimap advantage to dodge upcoming obstacles!


=============================== Game Functions ===============================

	* The minimap is obtained by rendering the scene in a smaller viewport,
using an orthogonal perspective;
	* The main camera moves and rotates at the same time with the car;
	* The check that the car is inside the road is done by checking that the
car center is in at least one of the triangles defining the road;
	* The obstacles collision check is done using spheres collision;
	* The race track is generated procedurally, using Geogebra for the points
that define the main shape and adding intermediary points to obtain more 
triangles;
	* The curvature of the landscape is done in the Vertex Shader, whilst
color is added in the Fragment Shader.

===============================================================================