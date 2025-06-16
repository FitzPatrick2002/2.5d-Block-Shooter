import GameManager;
import Combat;
import MainMenu;
import MapsMenuList;
import CreditsMenu;
import SettingsMenu;

import <iostream>;

// Comments DONE

GameManager::GameManager() {

	this->initGameOnStart();
}

GameManager::~GameManager() {

	delete this->mainWindow;
}

// 1. Creates the main window of the game
// 2. Adds the first state (MainMenu)
void GameManager::initGameOnStart() {

	this->mainWindow = new sf::RenderWindow(sf::VideoMode(1024, 768), "2.5D Shooter"); //, sf::Style::Default, settings
	//this->mainWindow->setFramerateLimit(60);

	this->createNewState(GameStateEnum::MainMenu, false);

}

// Update measures time elapsed between frames and passes it to the update function of the currently used state.
void GameManager::update() {
	
	while (this->mainWindow->isOpen()) {
		this->deltaTime = clock.restart();

		this->savedStates.top()->update(this->deltaTime);

	}
}

// When creating new states it's possible to either destroy the current state (pop the stack) or leave it. 
// After state is created on the heap, pointer to it is pushed onto the stack.
void GameManager::createNewState(GameStateEnum st, bool destackCurrent = true) {
	std::shared_ptr<GameState> temp;
	if (destackCurrent == true and not this->savedStates.empty()) {
		this->savedStates.pop();
	}

	switch (st) {
	case GameStateEnum::MainMenu:
		temp = std::make_shared<MainMenu>(this->mainWindow);
		this->savedStates.push(temp);

		break;
	case GameStateEnum::Combat:
		temp = std::make_shared<Combat>(this->mainWindow);
		this->savedStates.push(temp);

		break;
	case GameStateEnum::MapChoosing:
		temp = std::make_shared<MapsMenuList>(this->mainWindow);
		this->savedStates.push(temp);

		break;
	case GameStateEnum::Settings:
		temp = std::make_shared<SettingsMenu>(this->mainWindow);
		this->savedStates.push(temp);

		break;
	case GameStateEnum::Credits:
		temp = std::make_shared<CreditsMenu>(this->mainWindow);
		this->savedStates.push(temp);

		break;
	default:

		break;
	}
}