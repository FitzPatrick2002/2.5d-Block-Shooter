import Combat;
import linAlg;
import GameManager;
import MovementManager;
import GameMap;
import <chrono>;

// Comments DONE

// Constructor
// 1. Initialise the map (download data from file, build cuboids of which map consists.)
// 1.1 If map hasn't been chosen in the map menu, only player will be displayed and no map will be loaded.
// 2. Initialise the player view. Initially it covers the entire window. View is set to the window.
// 3. Enemies are initialised. Enemies manager handles the initialisation of enemies.
// 3.3. Name of the map is passed to it. The map file contains information where and how many enemies to spawn.
// 4. Movement manager needs information about the map. It takes care of the player movement and collitions of player with the surroundings.
Combat::Combat(sf::RenderWindow* win) : GameState(win) {
	this->initialiseMap();
	this->initPlayerView();
	this->initEnemies();
	MovementManager::getManager().setManagerData(&this->map);
}

// Destructor. 
// Set back the default view, when leaving the combat state.
Combat::~Combat() {
	this->window->getDefaultView();
}

// Downloads data from file located in Game/Maps and builds the map.
// 1. From the global object CombatMapData dowlnoad the currently selected map name.
// 2. Load the map from .txt file from Game/Maps folder
// 3. Rest is unused for now. Possible optimisiation

void Combat::initialiseMap() {

	std::string map_to_load = CombatMapData::getCombatMapData().getMapToLoad();

	// Make the .loadFromFIle() function of the map return boolean value
	// So that if it fails we can jump back to the main menu

	this->map.loadFromFile(map_to_load);
	this->map.initChunks();

	this->lines_buffer.setPrimitiveType(sf::Lines); // Unused.
	this->quads_buffer.setPrimitiveType(sf::Quads);

	this->map.batchBoxes(lines_buffer, quads_buffer); // Unused.

}

// Initialises the player view.
void Combat::initPlayerView() {
	this->playerView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	this->playerViewSize = sf::Vector2f(1024.0f, 768.0f);
	this->playerView.setSize(this->playerViewSize);
	this->playerViewScrollVal = 1.0f;
	this->window->setView(this->playerView);
}

// Iniltialises enemies based on the information from the currently chosen map.
// Initialisation is done by the enemies manager.
void Combat::initEnemies() {

	std::string map_to_load = CombatMapData::getCombatMapData().getMapToLoad(); // Get which map we want to load
	this->enemies_manager.accessSettings().readMapFile(map_to_load); // Open the map file and read number of enemies and their spawning points
	this->enemies_manager.init(&this->map, &this->player, &this->bullets);
}

// FOV is dependent of the viewing range of the player, fov angle (width) and the mouse position.
// Mouse position determines where the player is currently looking.
// The FOV algorithm uses a modified version of raycasting, which employs the bresenham line drawing algorithm.

// 1. Get the mouse position in the workd coordinates.
// 2. Calculate the angle from the y axis to the sight of line of the player.
// 3. Run the ray casting. It returns an unordered_map of positions which are visible
//		 Positions mean in this case, entire tiles. That is if something has coordinates (x, y) = (12.31, 6.07)
//		 Then the unordered_map receives value (12, 6), returned by the algorithm.
// 4. Pointers to map tiles (cuboids) from these positions are added to the displayed_objects vector
// 5. Pointers to enemies that are on these tiles are added to the displayed_objects vector.
// 6. Pointers to bullets that are on these tiles are added to the displayed_objects vector.
// 7. Sort the objects based on the distance from the camera. Ebtire game is in isometric view so we use x + y weights. 
//		Elements with larger values of x + y (closer to camera) are displayed first. 

void Combat::updatePlayerFOV() {	
	
	// 1. Get the mouse position in the workd coordinates.
	sf::Vector2f mouse_pos = this->getMousePos();
	mouse_pos = convertPixelsToTiles(mouse_pos);
	sf::Vector3f mousePos_3f = convertScreenInTilesToWorld(mouse_pos, 0.5f);
	float mouse_delta_x = mousePos_3f.x - player.getWorld_XY().x; // Calculate the vector from player to the mouse position in the world coordinates.
	float mouse_delta_y = mousePos_3f.y - player.getWorld_XY().y;
	
	float theta = getAngleOnPlane(mouse_delta_x, mouse_delta_y); // Angle between the x axis and the vector between the player and the mouse in the world.
	sf::Vector2i playerPos = V3_to_2i(this->player.getWorldPos());
	std::unordered_set<sf::Vector2i, Hashing::V2iHash> ground_to_display_s;

	// FOV width angle.
	float alpha = 3.14f / 4.0f;

	// New bresenham raycasting
	ground_to_display_s = Geometry::rayCastingBresenhamParallel(
		[this](int x, int y) -> bool {return this->map.checkIfTileWalkable(sf::Vector2i(x, y)); }, this->map.getWidth(), this->map.getHeight(),
		playerPos, 25.0f, theta, alpha);

	// Preapre the displayed_objects vector to be filled with visible elements.
	displayed_objects.clear();
	displayed_objects.resize(0);
	displayed_objects.resize(ground_to_display_s.size());

	// 4. Pointers to map tiles (cuboids) from these positions are added to the displayed_objects vector
	int i = 0;
	for (auto& pos : ground_to_display_s) {
		displayed_objects[i] = &(this->map.getTile(pos.x, pos.y));
		i++;
	}
	
	// 5. Pointers to enemies that are on these tiles are added to the displayed_objects vector.
	// Apparently some enemies appear to be invisible.
	int added_enemies_count = 0;
	displayed_objects.reserve(enemies.size() + bullets.size());
	for (auto& e : this->enemies) {
		
		if (ground_to_display_s.contains(v2f_to_v2i(e.second.getWorld_XY()))) {
			displayed_objects.emplace_back(&e.second);
			added_enemies_count++;
		}
	}

	// 6. Pointers to bullets that are on these tiles are added to the displayed_objects vector.
	for (auto& b : bullets) {
		if (ground_to_display_s.contains(v2f_to_v2i(b.getWorld_XY()))) {
			displayed_objects.emplace_back(&b);	
		}
	}
	

	displayed_objects.shrink_to_fit();

	// 7. Sort the objects based on the distance from the camera. Ebtire game is in isometric view so we use x + y weights. 
	auto comp_pts = [](const Entity* p1, const Entity* p2) {
		float sum_1 = p1->getWorld_XY().x + p1->getWorld_XY().y; // -.z
		float sum_2 = p2->getWorld_XY().x + p2->getWorld_XY().y; // -.z

		if (sum_1 == sum_2) {
			if (typeid(*p1) == typeid(Enemy) and typeid(*p2) == typeid(MapBox)) { // Put the enemies above the floor.
				return false;
			}
			if (typeid(*p1) == typeid(MapBox) and typeid(*p2) == typeid(Enemy)) { 
				return true;
			}
		}
		return sum_1 < sum_2;
		};

	std::ranges::sort(displayed_objects, comp_pts);
	
}

// Sets the current view size, based on the playerViewScrollVal * playerViewSize, latter being the (x,y) dimensions and the first one being the scale.
// View need to be updated every frame and is always centered about the player.

void Combat::updatePlayerView() {

	this->playerView.setSize(this->playerViewScrollVal * this->playerViewSize);

	this->window->setView(this->playerView);
	this->playerView.setCenter(this->player.getScreenPosInPixels());
}

// Handles the user input from the keyboard.
// Movement: 'wsad'.
// Movement is controlled by setting the velocity versor of the player in appropriate direction.
// Movement is relative to the world coordinates. So pressing 's' doesn't mean that the player will 
// move down the screen but will move along the y-axis (in the increasing coordinates direction).
// If none of the 'wsad' keays is pressed, velocity versor is set to (0,0,0).

void Combat::handleUserInput() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player.setVelVersor_Y(-1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.setVelVersor_X(1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.setVelVersor_Y(1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.setVelVersor_X(-1.0f);
	}

	if (not sf::Keyboard::isKeyPressed(sf::Keyboard::W) and not sf::Keyboard::isKeyPressed(sf::Keyboard::A) and not sf::Keyboard::isKeyPressed(sf::Keyboard::S) and not sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.setVelVersor(sf::Vector3f(0.0f, 0.0f, 0.0f));
	}
}

// Returns the mouse position on the screen, mapped to pixels (mapPixelsToCoords).
sf::Vector2f Combat::getMousePos() {
	sf::Vector2i mousePos;
	sf::Vector2f outcome;

	mousePos = sf::Mouse::getPosition(*this->window);
	outcome = this->window->mapPixelToCoords(mousePos);

	return outcome;
}

// Returns position of the mouse relative the center of the view.
sf::Vector2f Combat::getRawMousePosRelCenterNormalized() {
	sf::Vector2i mousePos;
	sf::Vector2f outcome;

	mousePos = sf::Mouse::getPosition(*this->window);
	outcome.x = (float)(mousePos.x) - (float)(this->window->getSize().x) / 2.0f; // Calculate vector from players position (center of the view) to the mouse.
	outcome.y = (float)(mousePos.y) - (float)(this->window->getSize().y) / 2.0f;

	return outcome;
}

// When left mouse button is clicked, bullet is created, initialised and put in the bullets list. 
// 1. Get the mouse position and calculate the direction in which the bullet will be flying (it's velocity versor).
// 2. Displace the bullet so that it is spaned outside the players tile (otherwise player would have shot himself).
// 3. Initilaise the bullet and put in the bullets list.

void Combat::createPlayersBullet() {
	Bullet b;

	// 1. Get the mouse position and calculate the direction in which the bullet will be flying (it's velocity versor).
	sf::Vector2f mouse_pos = convertPixelsToTiles(this->getMousePos());
	sf::Vector3f mouse_pos_3f = convertScreenInTilesToWorld(mouse_pos, 0.5f); // Put the bullet at the height z = 0.5f.

	// 2. Displace the bullet so that it is spaned outside the players tile (otherwise player would have shot himself).
	sf::Vector3f displacement = mouse_pos_3f - this->player.getWorldPos();
	displacement = makeVersor(displacement);
	sf::Vector3f bullet_starting_point = this->player.getWorldPos() + displacement;

	// 3. Initilaise the bullet and put in the bullets list.
	b.init(bullet_starting_point, convertPixelsToTiles(this->getRawMousePosRelCenterNormalized()));

	this->bullets.push_back(b);
}

// Handle user input from mouse and keyboard.
// When left mouse button is clicked, bulelt is spawned. 
// Keyboard input is handled in the handleUserInput(). Steering is on wsad.
// To go back to the main meny player must press 'm.
// On scroll the playerViewScrollVal is decreased by 0.1. 
// Negative values are allowed. It's a feature, not a bug.

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

// Inserts the bullet iterator into the bulletsToRemove unordered set.
void Combat::addBulletForRemoval(std::list<Bullet>::iterator& itr) {
	this->bulletsToRemove.insert(itr);
}

// Function that checks bullets collisions with all other objects (map boundaries, player, enemies and walls (cuboids)).
// 1. Iterate over all bullets in the bullets list.
//	1.1 Check if bullet is still on the map, if not, call addBulletForRemoval() (push it to the bulletsToRemove undoered_set.
//	1.2 If bullet is on a non-walkable tile (that's decided by the methode from GameMap), call addBulletForRemoval().
//	1.3 Iterate over the enemies and check if any enemy is on the same tiles as the bullet.
//		1.3.1 If so call addBulletForRemoval() and erase the enemy from the enemies unordered_multimap.
//	1.4 Check collsiion with player.
//		If collision occurs, decrease players health and call addBulletForRemoval().

void Combat::checkBulletsCollisions() {
	std::list<Bullet>::iterator itr = bullets.begin();

	for (itr; itr != bullets.end(); itr++) {
		bool removed = false;

		// If bullet flew out outside the map, delete it
		if (not this->map.checkIfOnMap(itr->getWorld_XY())) {
			this->addBulletForRemoval(itr);
			removed = true;
		}

		// If bullet hit a non-walkable tile (wall), delete it
		if (not this->map.checkIfTileWalkable(itr->getWorld_XY())) {
			this->addBulletForRemoval(itr);
			removed = true;
		}

		// Check collisions with enemies
		sf::Vector2i bullet_pos = v2f_to_v2i(itr->getWorld_XY());
		std::unordered_map <sf::Vector2i, Enemy, Vector2iHash>::iterator enemy_itr = this->enemies.find(bullet_pos);
		if (enemy_itr != this->enemies.end()) {
			this->addBulletForRemoval(itr);
			this->enemies.erase(enemy_itr);
			removed = true;
		}

		// Check collisions with player
		if (bullet_pos == v2f_to_v2i(player.getWorld_XY())) {

			//this->player->decreaseHp();
			this->player.decreaseHp(1);
			this->addBulletForRemoval(itr);
			removed = true;
		}
	}
}

// Checks all collisions.
// Calls only the 	checkBulletsCollisions() methode.
void Combat::checkCollisions() {
	this->checkBulletsCollisions();

}

// Loops over bullets in the bulletsToRemove unordered set and erases these bullets from the bullets list.
// Empties the bulletsToRemove set.
void Combat::removeObjects() {
	for (auto& e : bulletsToRemove) // This may cause iterator invalidation? Or smth, use std::remove_if or what not. Or just remove stuff inside the main loop
		bullets.erase(e);

	bulletsToRemove.clear();
}

// Updates the enemies positions and the enemies position in the enemies multimap.
// 1. Iterate over enemies
// 1.1. Compare the current postion (rounded to integer) with the previous position.
//		If it differs, schedule change in the enemies multimap (enemy needs to be trasnferred to another bukcet).
// 2. Rehash enemies that changed positions.

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

// Every time enemy is killed, enemies manager spawns an enemy, so that the amount of enemies is maximal.
// Spwaning stops when player killed enough enemies, then no new enemies are spawned and game ends when there are no enemies left.
void Combat::updateEnemySpawning() {
	this->enemies_manager.accessSettings().spawnEnemies(this->enemies);
}

// Main update function in the Combat state. It handles all updates in this state.
// 1. Handle user input from keyboard and mouse.
// 2. Update the player.
// 3. Update bullets.
// 4. Update enemies.
// 5. Handle collsitions.
// 6. Remove objects if any are colliding.
// 7. Spawn enemies.
// 8. Calcualte the player FOV. (Entier algorithm run once per frame).
// 9. Update the view.
// 10. Render all objects visible to player (from the displayed_objects vector).
// 11. Check end game conditions.

void Combat::update(sf::Time deltaTime) {

	//std::cout << "Fps: " << 1 / float(deltaTime.asSeconds()) << "\n";
	this->handleUserInput();

	MovementManager::getManager().movePlayer(this->player, deltaTime);
	this->player.update(deltaTime);
	this->player.updateSightAngle(this->getMousePos());
	
	for (auto& b : bullets) {
		b.update(deltaTime);
	}

	this->updateEnemies(deltaTime);
	this->checkCollisions();
	this->removeObjects();
	this->updateEnemySpawning(); // Recently added
	this->updatePlayerFOV();
	this->updatePlayerView();

	this->render();

	this->handleEvents();
	this->endCondition();
	//this->handleUserInput();

}

// Draws the visible elemnts from the displayed_objects vector into the window.
// Always draws the player.

void Combat::render() {
	this->window->clear();
	
	for (auto& e : this->displayed_objects) {
		e->render(window);
	}
	this->player.render(window);
	this->window->display();

}

// Used in case of resizing the window.
// Doesn't really work. Not bad but not properly either.
void Combat::resizeView() {
	sf::Vector2f windowSize = sf::Vector2f(float(this->window->getSize().x), float(this->window->getSize().y));

	float aspectRatio = windowSize.x / windowSize.y;

	sf::Vector2f playerView = this->playerViewScrollVal * this->playerViewSize;
	float viewRatio = this->playerViewSize.x / this->playerViewSize.y;

	this->playerViewSize = sf::Vector2f(playerView.x * aspectRatio, playerView.x);

}

// Check if the player hp is below zero. 
// If so go back to main menu.
// There are no win conditions for now. 
void Combat::endCondition() {

	if (this->player.getHp() < 0) {
		GameManager::getManager().createNewState(GameStateEnum::MainMenu, true);
	}

	if (this->enemies.size() == 0) {
		//GameManager::getManager().createNewState(GameStateEnum::MainMenu, true);
		//std::this_thread::sleep_for(std::chrono::microseconds(2500));
	}
}