import Combat;
import linAlg;
import GameManager;
import MovementManager;

import GameMap;

import <chrono>;

Combat::Combat(sf::RenderWindow* win) : GameState(win) {
	this->loadTextures();

	this->initialiseMap();
	this->initPlayerView();

	MovementManager::getManager().setManagerData(&this->map);
}

Combat::~Combat() {

	this->window->getDefaultView();
}

void Combat::initialiseMap() {

	std::string map_to_load = CombatMapData::getCombatMapData().getMapToLoad();

	// Make the .loadFromFIle() function of the map return boolean value
	// So that if it fails we can jump back to the main menu

	this->map.loadFromFile(map_to_load);
	this->map.initChunks();

	//this->map.init(50, 50, &this->textureManager);

}

void Combat::initPlayerView() {

	this->playerView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

	this->playerViewSize = sf::Vector2f(1024.0f, 768.0f);
	this->playerView.setSize(this->playerViewSize);

	this->playerViewScrollVal = 1.0f;

	this->window->setView(this->playerView);
}

void Combat::updatePlayerFOV() {

	this->map.setPlayerFOV(this->player.getWorldPos(), this->getRawMousePosRelCenterNormalized());
	this->map.setRenderOrder(/*this->player.getWorldPos() */);
}

void Combat::updatePlayerView() {

	this->playerView.setSize(this->playerViewScrollVal * this->playerViewSize);

	this->window->setView(this->playerView);
	this->playerView.setCenter(this->player.getScreenPosInPixels());
}


void Combat::createWindow() {
	this->window = new sf::RenderWindow(sf::VideoMode(1024, 768), "RPG");
}

void Combat::loadTextures() {

	textureManager.addTexture("eyes", "Graphics/Eyes.png");
	textureManager.addTexture("Darius", "Graphics/Dancing.png");
	textureManager.addTexture("Bullet", "Graphics/Bullet.png");
	textureManager.addTexture("Tile_40x40", "Graphics/Tile_40x40.png");

}

void Combat::handleUserInput() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//player.setVelVersor(sf::Vector2f(0.0f, -1.0f));
		player.setVelVersor_Y(-1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//player.setVelVersor(sf::Vector2f(1.0f, 0.0f));
		player.setVelVersor_X(1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//player.setVelVersor(sf::Vector2f(0.0f, 1.0f));
		player.setVelVersor_Y(1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//player.setVelVersor(sf::Vector2f(-1.0f, 0.0f));
		player.setVelVersor_X(-1.0f);
	}

	if (not sf::Keyboard::isKeyPressed(sf::Keyboard::W) and not sf::Keyboard::isKeyPressed(sf::Keyboard::A) and not sf::Keyboard::isKeyPressed(sf::Keyboard::S) and not sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.setVelVersor(sf::Vector3f(0.0f, 0.0f, 0.0f));
	}

	// Use this -> just set some timer that counts time between shots (depending on the rifle used)

	//if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
	//	this->createPlayersBullet();
	//}

}

// Returns the position in pixels (converted to world coordinates with MapToPixels or smth.)
sf::Vector2f Combat::getMousePos() {
	sf::Vector2i mousePos;
	sf::Vector2f outcome;

	mousePos = sf::Mouse::getPosition(*this->window);
	//std::cout << "Mouse pos: " << mousePos.x<<" "<<mousePos.y << "\n";

	outcome = this->window->mapPixelToCoords(mousePos);

	return outcome;
}

// Returns position of the mouse relative the center of the view (it's not normalised actaully)
sf::Vector2f Combat::getRawMousePosRelCenterNormalized() {
	sf::Vector2i mousePos;
	sf::Vector2f outcome;

	mousePos = sf::Mouse::getPosition(*this->window);
	outcome.x = (float)(mousePos.x) - (float)(this->window->getSize().x) / 2.0f;
	outcome.y = (float)(mousePos.y) - (float)(this->window->getSize().y) / 2.0f;
	//std::cout << "Mouse pos: " << outcome << "\n";
	//outcome.x /= (float)(this->window->getSize().x);
	//outcome.y /= (float)(this->window->getSize().y);

	return outcome;
}

void Combat::createPlayersBullet() {
	Bullet b;
	b.init(this->player.getWorldPos(), convertPixelsToTiles(this->getRawMousePosRelCenterNormalized()));

	this->bullets.push_back(b);
}

void Combat::handleEvents() {

	while (this->window->pollEvent(this->stateEvent)) {

		switch (this->stateEvent.type) {
		case sf::Event::Closed():
			this->window->close();
			break;
		case sf::Event::MouseMoved:

			break;
		case sf::Event::MouseButtonPressed:
			this->createPlayersBullet();
			break;
		case sf::Event::MouseWheelScrolled:

			this->playerViewScrollVal -= float(this->stateEvent.mouseWheelScroll.delta) / 10.0f;
			break;
		case sf::Event::KeyPressed:
			if (this->stateEvent.key.code == sf::Keyboard::M) {
				GameManager::getManager().createNewState(GameStateEnum::MainMenu, true);
				return;
			}
			break;
		case sf::Event::Resized:
			this->resizeView();
			break;
		}
	}

}

void Combat::addBulletForRemoval(std::list<Bullet>::iterator& itr) {

	this->bulletsToRemove.insert(itr);
}

void Combat::checkBulletsCollisions() {

	std::list<Bullet>::iterator itr = bullets.begin();
	std::list<Bullet>::iterator itr_2 = bullets.begin();

	for (itr; itr != bullets.end(); itr++) {

		if (not this->map.checkIfOnMap(itr->getWorld_XY())) {
			this->addBulletForRemoval(itr);
		}

		if (not this->map.checkIfTileWalkable(itr->getWorld_XY()))
			this->addBulletForRemoval(itr);
	}
}

void Combat::checkCollisions() {

	this->checkBulletsCollisions();

}

void Combat::removeObjects() {
	for (auto& e : bulletsToRemove)
		bullets.erase(e);

	bulletsToRemove.clear();


}

// Here we put all the stuff that does not regard rendering and graphics
// User input, Updating positions, physics, etc.
void Combat::update(sf::Time deltaTime) {

	//std::cout << "Fps: " << 1 / float(deltaTime.asSeconds()) << "\n";

	//std::cout << this->player.getWorldPos().x<< ", "<<this->player.getWorldPos().y << "\n";
	this->handleUserInput();

	MovementManager::getManager().movePlayer(this->player, deltaTime);

	this->player.update(deltaTime);
	this->player.updateSightAngle(this->getMousePos());
	this->updatePlayerFOV();

	for (auto& b : bullets) {
		b.update(deltaTime);
	}

	this->checkCollisions();

	this->removeObjects();

	this->updatePlayerView();
	this->render();

	this->handleEvents();
	this->handleUserInput();

}

// Here we only draw things onto the
void Combat::render() {
	this->window->clear();

	this->map.render(this->window);

	for (auto& e : bullets)
		e.render(window);

	this->player.render(window);

	this->window->display();

}

void Combat::resizeView() {
	sf::Vector2f windowSize = sf::Vector2f(float(this->window->getSize().x), float(this->window->getSize().y));

	float aspectRatio = windowSize.x / windowSize.y;

	sf::Vector2f playerView = this->playerViewScrollVal * this->playerViewSize;
	float viewRatio = this->playerViewSize.x / this->playerViewSize.y;

	this->playerViewSize = sf::Vector2f(playerView.x * aspectRatio, playerView.x);

}