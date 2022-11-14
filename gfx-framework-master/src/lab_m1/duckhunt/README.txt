// Boldeanu Ana-Maria
// EGC - Homework 1 - Duck Hunt


================================ Description ================================

	This game is a simplified version of the classic Duck Hunt, a 2D where
players shoot ducks that appear on the screen from a first-person perspective.

	* The player starts with 3 lives;
	* Ducks appear one at a time and the player is allowed 3 bullets to shoot 
each one;
	* The player can click the mouse to shoot a bullet at the cursor location
(if it collides with the duck model, it's a hit!);
	* If the player fails to hit the duck (runs out of bullets) or a certain time
has passed, the duck evades and the player loses 1 life;
	* Upon losing 3 lives, the game is over (though it can be reset without
re-running)
	* Every 5 spawned ducks, the flight speed increases and it gets harder to hit
them;
	* When the duck collides with the window borders, it reflects back;
	* Each hit adds some points to the current score; reach max score to win!
	* The faster the duck flies, the more points the player gets for hitting it;
	* Game interface elements:
- Current lives and score (top left);
- Current number of bullets (top right);
- Some clouds;
- Two layers of grass (bottom) - The duck spawns behind the front layer;
- Sky (background color) changing color according to round state;
- A cute & fearless, flying duck.
	

================================ How to play ================================

	* (dev step) Make sure that src/main.cpp runs the DuckHunt program
i.e. World* world = new m1::DuckHunt();
	* Run the program;
	* Press R to start the game or to reset it at any time;
	* Click the mouse to shoot a bullet at the cursor position;
	* Shoot those fearless ducks!!


=============================== Game Functions ===============================

* Duck & Flight
	- The duck has 3 possible states: Active, Evaded and Dead.

	- When Active & Evaded, the duck has a wings-flapping animation.

	- When Active, the duck starts at a random position behind the grass at the
bottom of the screen, flying upwards in a random direction (i.e. with a random 
rotation angle) - The sky is blue!

	- The rotation angle and the duck model are flipped ever time the duck hits
a window border, changing its flying direction so that it doesn't fly outside 
the window.

	- When Evaded (round time limit reached or player out of bullets), the duck
flights upward at a 90 degrees angle - The sky turns red :(

	- When Dead (duck was shot), the duck falls down and the wings stop flapping
- The sky turns green!


* Interface
	- Score bar is shown as a wireframe box and gets filled as the player hits
ducks.

	- Number of lives and bullets are updates as the game goes on.


* DuckHunt
	- Window collisions calculated using current duck position relative to window
resolution limits.

	- Duck collision upon clicking calculated using the mouse position, 
transformed (translated and rotated) so that it corresponds to the initial system
of coordinates for the duck, where its body has the hitbox of a rectangle.

	- Frame update animations are calculated using the current deltaTime, to 
determine the distance that the duck has travelled inside it.

	- Some text is rendered to indicate when the game is won/lost and how to
restart.

===============================================================================