export module GameState;

import <SFML/Graphics.hpp>;

// Enum with all possible states that can appear in the game.
// Numbered from 0.
export enum class GameStateEnum
{
	MainMenu = 0,
	Combat,
	MapChoosing,
	Settings,
	Credits,
	Exit
};

// GameState is an abstract class, from which all game states inherit. They are listed in the enum above. 
// Game sate describes what is currently displayed in the window. It handles interactions with the user 
// and rendering of the scene. It stores everything that is visible on the screen.

// States can be created by the GameManager (singleton). After creation state is pushed onto the states stack.
// When user leaves the state (for example exists the settings and goes back to the main menu, current state 
// is destroyed. However when we are stacking more states onto one another, the prvious state doesn't necessairly be destroyed. 
// Only state on top of the stack is updated.

export class GameState{
protected:
	sf::RenderWindow* window; // Window is created in the GameManger. Pointer to it is held by every state. 
	sf::Event stateEvent;

public:

	GameState(sf::RenderWindow* win) {
		this->window = win;
		this->window->setView(this->window->getDefaultView());
	}

	~GameState(){

	}

	void handleUserInput();

	virtual void update(sf::Time deltaTime) = 0;
};