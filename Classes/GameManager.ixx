export module GameManager;

import GameState;
import Combat;
import <stack>;
import <memory>;

import <SFML/Graphics.hpp>;
// TO DO: When using threads -> make sure they all finish their jobs before we destroy something

export class GameManager {
private:

	std::stack<std::shared_ptr<GameState>> savedStates; // States saved on stack. We can add new ones on top and , delete tthem once we're done doing what we wanted and then comeback to the old ones

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