import EnemiesManager;

import <SFML/Graphics.hpp>;

import Enemy;
import EnemyCommands;
import linAlg;

EnemiesManager::EnemiesManager() {

}

EnemiesManager::~EnemiesManager() {

}

void EnemiesManager::init(GameMap* gameMap_handle) {
	this->gameMap_handle = gameMap_handle;
}

// For starters:
	// 1. Check which command is being performed
	// 2. If it is the wait command -> queue new commands
	//		2.1. Choose random point on map
	//		2.2. Calculate the velocity versor to this point 
	//		2.3. Queue the appropriate commands
	// Else do nothing
void EnemiesManager::updateEnemy(Enemy& e) {
	sf::Vector2f new_destination;
	sf::Vector2f new_velocity_versor;

	if (e.commandQueueEmpty()) {

		new_destination = random_V2f(0.0f, (float)(this->gameMap_handle->getWidth() - 1), 0.0f, (float)(this->gameMap_handle->getHeight() - 1));
		new_velocity_versor = makeVersor(e.getWorld_XY(), new_destination);

		//std::cout << "New destination: " << new_destination << "\n";

		Start_movement mov(new_velocity_versor, 2.5f);
		Check_if_at_destination dest(new_destination);
		Stop_movement stop;

		e.queueCommand(&mov);
		e.queueCommand(&dest);
		e.queueCommand(&stop);
		/*
		Start_movement mov(sf::Vector2f(1.0f, 0.0f), 2.0f);
	Check_if_at_destination dest(sf::Vector2f(10.0f, 0.0f));

	Stop_movement stop;
	mov.getNewVelocityMagnitude();

	enemy.queueCommand(&mov);
	enemy.queueCommand(&dest);
	enemy.queueCommand(&stop);
		*/
	}


}