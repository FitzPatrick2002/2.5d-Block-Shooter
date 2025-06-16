export module GameManager;

import GameState;
import Combat;
import <stack>;
import <memory>;

import <SFML/Graphics.hpp>;

// Comments DONE

// Class used to govern the states. 
// When player clicks on a tile in the main menu a new state is created and added to stack. 
// When the state is not used anymore, the stack of states is popped and stae that is currently on top is used (updated).
//In order to go back to main menu plyer need to press the 'm' button.
export class GameManager {
private:

	// States saved on stack. We can add new ones on top and delete them once we're done doing what we wanted and then go back to the old ones
	std::stack<std::shared_ptr<GameState>> savedStates;

	// The main window of the entire application. 
	// Everything happens in it.
	// States receive pointer to it.
	sf::RenderWindow* mainWindow;
	sf::ContextSettings settings;

	// Time managemenet. Used mainly for measuring time between frames.
	sf::Clock clock;
	sf::Time deltaTime;

public:

	GameManager();
	~GameManager();

	static GameManager& getManager() {
		static GameManager gameManager;

		return gameManager;
	}

	void initGameOnStart();

	void update();

	void createNewState(GameStateEnum st, bool destackCurrent);
};