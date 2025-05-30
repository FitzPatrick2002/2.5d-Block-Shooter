export module EnemiesManager;

import <SFML/Graphics.hpp>;
import Enemy;
import EnemyCommands;

import linAlg;
import GameMap;
import Bullet;

import Player;

import <iostream>;
import <thread>;
import <mutex>;
import <list>;
import <vector>;
import <unordered_map>;
import <fstream>;
import <regex>;
import <filesystem>;
import <functional>;

struct EnemiesOnMapSettings {
private:
	std::vector<sf::Vector2f> spawners;

	int available_enemies = 0;
	int current_enemies = 0; // Stores the number of enemies form the previous frame

public:

	EnemiesOnMapSettings() = default;
	~EnemiesOnMapSettings() {

	}

	void spawnEnemies(std::unordered_multimap<sf::Vector2i, Enemy, Vector2iHash>& enemies_container) {
		int delta_enemies = current_enemies - enemies_container.size(); // How many enemies were killed
		//std::cout << "Delta enemies: " << delta_enemies << "\n";
		std::cout << "enemies number: " << enemies_container.size() << "\n";

		// Spawn the missing enemies, if there are no available enemies, don't spawn.
		// 1. Decrease the number of available enemies (how many left for that map)
		// 2. Create dummy enemy that will be pushed into the map
		// 3. Choose position for this enemy (random)
		// 4. Push enemy to map and initialise it.

		/*
		sf::Vector3f position(2, 2 + (i * 2), 0); // Position of enemy
		Enemy e;

		this->enemies.insert({sf::Vector2i(position.x, position.y), e}); // Add enemy with this position
		this->enemies.find(V3_to_2i(position))->second.init(position); // Initialise this enemy. (Already existing on the map, in the map of enemies)
		*/

		while (delta_enemies > 0 and available_enemies > 0 and delta_enemies-- > 0) {
			available_enemies--;

			Enemy e;
			int pos_num = get_random_int(0, spawners.size()-1);
			sf::Vector3f enemy_pos(spawners[pos_num].x, spawners[pos_num].y, 0.0f);

			enemies_container.insert({ sf::Vector2i(enemy_pos.x, enemy_pos.y), e});
			enemies_container.find(V3_to_2i(enemy_pos))->second.init(enemy_pos);

		}
	}

	void readMapFile(std::string map_name) {
		std::ifstream map_file("Game\\Maps\\" + map_name + ".txt");

		// Read the number of enemies and positions at which they can spawn
		if (map_file.is_open()) {
			std::string line;

			std::regex enemies_num_regex(R"((Enemies:)\s*(\d+))");
			std::regex spawners_line_regex(R"((Spawners:))");
			std::regex spwaners_pts_regex(R"(\((\d+)[,]?\s?(\d+)\)[,]?\s?)");


			this->available_enemies = -1;
			while (std::getline(map_file, line)) {
				std::smatch m;

				// Find the number of enemies
				if (std::regex_search(line, m, enemies_num_regex)) {
					this->available_enemies = std::atoi(m[2].str().c_str());
					this->current_enemies = this->available_enemies / 3;
				}

				// Get the spawning points
				if (std::regex_search(line, m, spawners_line_regex)) {
					auto begin = std::sregex_iterator(line.begin(), line.end(), spwaners_pts_regex);
					auto end = std::sregex_iterator();

					for (std::sregex_iterator i = begin; i != end; ++i) {
						std::smatch point = *i;

						if (point.size() == 3) {
							int x = std::stoi(point[1]); // Or 1,2 dunno
							int y = std::stoi(point[2]);

							this->spawners.push_back(sf::Vector2f(x, y));
						}
						else {
							std::cerr << "Some spawning points are wrongly formatted. Should be: (int, int).\nIntegers must be non negative.\n";
						}
					}

				}
			}
		}
		else {
			std::cerr << "Could not open the map file\n";
		}
	}

};

export class EnemiesManager {
private:
	GameMap* gameMap_handle;
	Player* player_handle;

	std::list<Bullet>* bullets_list_handle;
	std::mutex bullets_mutex;

	EnemiesOnMapSettings enemies_map_settings;

public:

private:

	bool checkIfEnemySeesPlayer(Enemy& e);

public:

	EnemiesManager();
	~EnemiesManager();

	void init(GameMap* gameMap_handle, Player* p, std::list<Bullet>* blh);

	EnemiesOnMapSettings& accessSettings() {
		return this->enemies_map_settings;
	}

	// For starters:
	// 1. Check which command is being performed
	// 2. If it is the wait command -> queue new commands
	//		2.1. Choose random point on map
	//		2.2. Calculate the velocity versor to this point 
	//		2.3. Queue the appropriate commands
	// Else do nothing
	void updateEnemy(Enemy& e);
};