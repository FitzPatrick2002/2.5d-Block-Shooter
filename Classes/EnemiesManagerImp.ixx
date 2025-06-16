import EnemiesManager;

import <SFML/Graphics.hpp>;

import Enemy;
import EnemyCommands;
import linAlg;
import Bullet;

import <functional>;
import <list>;
import <iostream>;

// Private:

bool EnemiesManager::checkIfEnemySeesPlayer(Enemy& e) {
	sf::Vector2f enemy_pos = e.getWorld_XY();
	sf::Vector2f player_pos = player_handle->getWorld_XY();
	float dist = vectorLength(player_pos - enemy_pos);

	//std::cout << "Player pos: " << player_pos << "\n";

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

void EnemiesManager::init(GameMap* gameMap_handle, Player* p, std::list<Bullet>* blh) {
	this->gameMap_handle = gameMap_handle;
	this->player_handle = p;
	this->bullets_list_handle = blh;
}

// Okay, a bit more sophiscicated

// Patrol:
// 0. Reset the commands list
// 1. Choose some random point on the map
// 2. Run A* to find the path
// 3. Queue commands to get to the beginning of the path
// 4. Queue commands for traversing the entire path  (n points -> (n - 1) commands batches (Start, Travel_to_..., Stop))

// Combat:
// 0. Clear the commands list
// 1. Shooting
//		- Make shot
//		- Start timer
//		- Check if enough time passed
//		- Check if shot can be made -> Already done when changng states Combat <-> Patrol


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
			std::function<bool(sf::Vector2i)> isWalkableFunc = [&](sf::Vector2i pos) -> bool {
				return this->gameMap_handle->checkIfTileWalkable(pos);
				};
			//std::cout << "Here\n";
			sf::Vector2i enemy_pos = v2f_to_v2i(e.getWorld_XY());

			sf::Vector2i rand_end_pos = getRandomWalkable(isWalkableFunc, sf::Vector2i(0, this->gameMap_handle->getWidth() - 1), sf::Vector2i(0, this->gameMap_handle->getHeight()));

			std::vector<sf::Vector2i> path = A_starSearch(isWalkableFunc, enemy_pos, rand_end_pos);

			sf::Vector2f new_vel;
			float vel_mag = CombatMapData::getCombatMapData().getEnemySpeed();

			if (e.getWorld_XY() != v2i_to_v2f(path[0])) {
				new_vel = makeVersor(e.getWorld_XY(), v2i_to_v2f(path[0]));

				Start_movement start(new_vel, vel_mag);
				Check_if_at_destination travel(v2i_to_v2f(path[0]));
				Stop_movement stop;

				e.queueCommand(&start);
				e.queueCommand(&travel);
				e.queueCommand(&stop);
			}

			for (int i = 1; i < path.size(); i++) {

				new_vel = makeVersor(v2i_to_v2f(path[i] - path[i - 1]));

				Start_movement start(new_vel, vel_mag);
				Check_if_at_destination travel(v2i_to_v2f(path[i]));
				Stop_movement stop;

				e.queueCommand(&start);
				e.queueCommand(&travel);
				e.queueCommand(&stop);
			}

		}
		else {
			// Do nothing and let the enemy work by himself
		}
	}
	else if (e.getStatus() == EnemyState::Combat) {
		if (e.commandQueueEmpty() == true) {
			Shoot shot(this->bullets_list_handle, this->player_handle->getWorld_XY());
			Wait wait(1.0f);

			e.queueCommand(&shot);
			e.queueCommand(&wait);
		}
		else {

		}
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