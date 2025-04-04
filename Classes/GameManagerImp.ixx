import GameManager;
import Combat;
import MainMenu;
import MapsMenuList;

import <iostream>;

GameManager::GameManager() {

	this->initGameOnStart();
}

GameManager::~GameManager() {

	delete this->mainWindow;
}

// TO DO: Make it start with the MainMenu
void GameManager::initGameOnStart() {

	this->mainWindow = new sf::RenderWindow(sf::VideoMode(1024, 768), "2.5D Shooter"); //, sf::Style::Default, settings
	//this->mainWindow->setFramerateLimit(60);

	this->createNewState(GameStateEnum::MainMenu, false);

}

void GameManager::update() {
	
	while (this->mainWindow->isOpen()) {
		this->deltaTime = clock.restart();

		this->savedStates.top()->update(this->deltaTime);

	}
}

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
	case GameStateEnum::Credits:

		break;
	default:

		break;
	}
}