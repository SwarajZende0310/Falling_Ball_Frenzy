#include"Game.h"

int main() {
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//Init Game engine
	Game game;

	//Game Loop
	while (game.running() && !game.getEndgame()) {
		//Update
		game.update();

		//Render
		game.render();
	}

	game.gameOverText();
	//End of Application

	return 0;
}