// Boldeanu Ana-Maria
// EGC - Homework 3 - SkiFree 3D


================================ Description ================================

	This game is a simplified version of SkiFree, a 3D game where the
player uses the mouse to control a skier on a mountain slope, collecting gifts
and avoiding obstacles.

Features
	* Colourful scene, decorated by pine trees, street lamps and rocks.
	* The player has 3 lives, displayed on top of the skier's head.
	* The skier is slowed upon obstacle collision.
	* There are collectible gifts on the mountain slope.
	

================================ How to play ================================

	* (dev step) Make sure that src/main.cpp runs the SkiFree program
i.e. World* world = new m1::SkiFree();
	* Run the program;
	* Use the mouse to rotate the skier towards left or right as you 
advance down the slope.
	* Try to get as many gifts as you can before hitting an obstacle!


=============================== Game Functions ===============================

	* All the objects used to decorate the scene are made using 3D model 
transformations on primary meshes such as spheres and cubes.
	* The obstacles are randomly generated as the skier is advancing
down the slope.
	* The obstacles are removed from the scene when they leave the viewport.
	* Textures and illumination are added on each object and computed using
the Vertex and Fragment shaders.

===============================================================================