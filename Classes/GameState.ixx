export module GameState;

import <SFML/Graphics.hpp>;

export enum class GameStateEnum
{
	MainMenu = 0,
	Combat,
	MapChoosing,
	Settings,
	Credits,
	Exit
};

export class GameState{
protected:

	sf::RenderWindow* window;
	sf::Event stateEvent;

public:

	GameState(sf::RenderWindow* win) {
		this->window = win;
		this->window->setView(this->window->getDefaultView());
	}
	~GameState(){

		//delete this->window;
	}

	void handleUserInput();

	virtual void update(sf::Time deltaTime) = 0;
};