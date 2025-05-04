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

	this->initEnemies();

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

	//this->map.init(50, 50, &this->textureManager); // Leave this as is

	this->lines_buffer.setPrimitiveType(sf::Lines);
	this->quads_buffer.setPrimitiveType(sf::Quads);

	this->map.batchBoxes(lines_buffer, quads_buffer);

}

void Combat::initPlayerView() {

	this->playerView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

	this->playerViewSize = sf::Vector2f(1024.0f, 768.0f);
	this->playerView.setSize(this->playerViewSize);

	this->playerViewScrollVal = 1.0f;

	this->window->setView(this->playerView);
}

void Combat::initEnemies() {

	for (int i = 0; i < 20; i++) {
		sf::Vector3f position(2, 2 + (i * 2), 0);
		Enemy e;

		Start_movement mov(sf::Vector2f(1.0f, 0.0f), 2.0f);
		Check_if_at_destination dest(sf::Vector2f(10.0f, 0.0f));

		Stop_movement stop;

		this->enemies.insert({sf::Vector2i(position.x, position.y), e});
		
		this->enemies.find(V3_to_2i(position))->second.init(position);
		//this->enemies[V3_to_2i(position)].init(position);

	//	this->enemies[V3_to_2i(position)].queueCommand(&mov);
	//	this->enemies[V3_to_2i(position)].queueCommand(&dest);
	//	this->enemies[V3_to_2i(position)].queueCommand(&stop);
	}

	this->enemies_manager.init(&this->map, &this->player, &this->bullets);
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
		bool removed = false;

		if (not this->map.checkIfOnMap(itr->getWorld_XY())) {
			this->addBulletForRemoval(itr);
			//itr = this->bullets.erase(itr);
			removed = true;
		}

		if (not this->map.checkIfTileWalkable(itr->getWorld_XY())) {
			this->addBulletForRemoval(itr);
			//itr = this->bullets.erase(itr);
			removed = true;
		}

		// Check collisions with enemies
		sf::Vector2i bullet_pos = v2f_to_v2i(itr->getWorld_XY());
		std::unordered_map <sf::Vector2i, Enemy, Vector2iHash>::iterator enemy_itr = this->enemies.find(bullet_pos);
		if (enemy_itr != this->enemies.end()) {
			
			this->addBulletForRemoval(itr);
			//itr = this->bullets.erase(itr);
			this->enemies.erase(enemy_itr);
			
			removed = true;
		}

		//if (not removed) {
		//	++itr;
		//}


	}
}

void Combat::checkEnemiesCollisions() {
	// Apparently everything is done in the bullets collisions
}

void Combat::checkCollisions() {

	this->checkBulletsCollisions();

}

void Combat::removeObjects() {
	for (auto& e : bulletsToRemove) // This may cause iterator invalidation? Or smth, use std::remove_if or what not. Or just remove stuff inside the main loop
		bullets.erase(e);

	bulletsToRemove.clear();


}

void Combat::updateEnemies(sf::Time deltaTime) {

	std::vector<std::tuple<sf::Vector2i, sf::Vector2i, Enemy*>> positions_for_change;

	for (auto& e : enemies) { 
		
		sf::Vector2i previous_position = e.first; // Compare the current (int) position with the one after updating the position, if these positions differ we need to store this element under in different bucket
		e.second.update(deltaTime); 
		sf::Vector2i current_position = V3_to_2i(e.second.getWorldPos());

		// If position of the enemy changed, save the info about the current key, new key and the enemy whose position is to be changed
		if (current_position != previous_position) {
			positions_for_change.push_back({ previous_position, current_position, &e.second });
		}

		/*this->threadPool.queueJob([&, this]() {
			this->enemies_manager.updateEnemy(e.second);
			});*/

		//while (this->threadPool.busy()) {
			//std::cout << "Waiting\n";
		//}
		this->enemies_manager.updateEnemy(e.second);
		
	}

	// Iterate over all the enemeis that need rehashing
	for (const auto& [prev, new_pos, enemy] : positions_for_change) {

		auto range = enemies.equal_range(prev); // get all the enemies from that particular bucket
		for (auto it = range.first; it != range.second; it++) {

			if (&it->second == enemy) {// If it is the right enemy change the key, and insert the enemy anew with a new key
				enemies.insert({ new_pos, std::move(it->second) });
				enemies.erase(it);
				break; // Once we've updated the enemy there is no need to iterate further, we would get an iterator invalidation error anyways
			}
		}
	}
}

// Here we put all the stuff that does not regard rendering and graphics
// User input, Updating positions, physics, etc.
void Combat::update(sf::Time deltaTime) {

	std::cout << "Fps: " << 1 / float(deltaTime.asSeconds()) << "\n";

	//std::cout << this->player.getWorldPos().x<< ", "<<this->player.getWorldPos().y << "\n";
	this->handleUserInput();

	MovementManager::getManager().movePlayer(this->player, deltaTime);

	this->player.update(deltaTime);
	this->player.updateSightAngle(this->getMousePos());
	this->updatePlayerFOV();

	for (auto& b : bullets) {
		b.update(deltaTime);
	}

	this->updateEnemies(deltaTime);

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
	//this->window->draw(this->quads_buffer);
	//this->window->draw(this->lines_buffer);
	

	for (auto& e : bullets)
		e.render(window);

	for (auto& e : enemies)
		e.second.render(window);

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