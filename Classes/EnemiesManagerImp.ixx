import EnemiesManager;

import <SFML/Graphics.hpp>;

import Enemy;
import EnemyCommands;
import linAlg;

import <iostream>;

// Private:

bool EnemiesManager::checkIfEnemySeesPlayer(Enemy& e) {
	sf::Vector2f enemy_pos = e.getWorld_XY();
	sf::Vector2f player_pos = player_handle->getWorld_XY();
	float dist = vectorLength(player_pos - enemy_pos);

	//std::cout << "Plauyer pos: " << player_pos << "\n";

	// Cast the ray
	if (dist < e.getViewRange()) {
		sf::Vector2f versor = makeVersor(player_pos - enemy_pos);
		float length = 0.0f;
		float delta = 0.1f;

		while (length < dist) {
			length += delta;

			if (not this->gameMap_handle->checkIfTileWalkable(e.getWorld_XY() + length * versor)) {
				return false; // Something is obstructing the view -> can't see
			}
			else {
				sf::Vector2i map_pos = v2f_to_v2i(length * versor);
				sf::Vector2i p_pos = v2f_to_v2i(player_pos);

				if (map_pos == p_pos)
					return true;
			}
		}

		return true; // The sight line is clear and unimpeded -> can see the player
	}

	return false; // Out of range -> can't see
}

// Public:

EnemiesManager::EnemiesManager() {

}

EnemiesManager::~EnemiesManager() {

}

void EnemiesManager::init(GameMap* gameMap_handle, Player* p) {
	this->gameMap_handle = gameMap_handle;
	this->player_handle = p;
}

// Okay, a bit more sophiscicated

void EnemiesManager::updateEnemy(Enemy& e) {

	// If player visible set status to combat, if not, set it to patrol
	if (checkIfEnemySeesPlayer(e)) {
		if (e.getStatus() == EnemyState::Patrol) {
			e.setStatus(EnemyState::Combat);
			e.clearCommandsQueue();
		}
	}
	else {
		if (e.getStatus() == EnemyState::Combat) {
			e.setStatus(EnemyState::Patrol);
			e.clearCommandsQueue();
		}
	} // Check this part less frequently actually -> have a separate counter

	//std::cout << "Status: " << static_cast<int>(e.getStatus()) << "\n";

	if (e.getStatus() == EnemyState::Patrol) {
		if (e.commandQueueEmpty() == true) {
			// Choose random point and set up a path to it
			// Queue the commands that will ge the enemy to the destination
		}
		else {
			// Do nothing and let the enemy work by himself
		}
	}
	else if (e.getStatus() == EnemyState::Combat) {

	}

	/*if (e.getStatus() == EnemyState::Patrol) {
		if (e.commandQueueEmpty() == true) {
			// Choose random point and set up a path to it
			// Queue the commands that will ge the enemy to the destination
		}
		else {
			// Do nothing and let the enemy work by himself
		}
	}
	else if (e.getStatus() == EnemyState::Combat) {

	}*/



	/*sf::Vector2f new_destination;
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

	}*/


}