import <iostream>;
import <SFML/Graphics.hpp>;

import GameManager;
import linAlg;
import <functional>;

// TO DO:
// 1. Make enemies visible (DONE)
// 2. Store them in a list and update them (use the movement manager) (DONE) -> not list an unordered_map keys - positions
// 3. Detect collisions between bullets and enemies (DONE
// 4. Add AI for enemies -> EnemiesManager -> for now very simple just patrolling, no states
// 5. Make the EnemyManager run on threads / Update enemies using threads -> their update functions may get lengthy?

import <queue>;
import <thread>;
import <mutex>;
import <functional>;
import <vector>;
import <unordered_map>;
import <unordered_set>;

import Enemy;
import <ranges>;

struct TempMap {

	std::string arr[10] = {
		"..........",
		"########..",
		"..........",
		"..#.#.#...",
		".........#",
		".#.####...",
		"......#...",
		"......#...",
		"..........",
		".........."
	};

	bool isInBounds(sf::Vector2i pos) {
		if (pos.x < 0 or pos.x >= 10)
			return false;

		if (pos.y < 0 or pos.y >= 10)
			return false;

		return true;
	}

	bool isWalkable(sf::Vector2i pos) {
		if (not isInBounds(pos))
			return false;

		if (arr[pos.y][pos.x] == '#')
			return false;
		
		return true;
	}

};

int main()
{
	/*
	Enemy enemy;
	
	enemy.setWorldPos(sf::Vector3f(0, 0, 0));
	enemy.setVelMag(0);
	enemy.setVelVersor(sf::Vector3f(0, 0, 0));

	sf::Clock clk;
	sf::Time deltaTime;

	Start_movement mov(sf::Vector2f(1.0f, 0.0f), 2.0f);
	Check_if_at_destination dest(sf::Vector2f(10.0f, 0.0f));
	
	Stop_movement stop;
	mov.getNewVelocityMagnitude();

	enemy.queueCommand(&mov);
	enemy.queueCommand(&dest);
	enemy.queueCommand(&stop);
	
	while (true) {
		deltaTime = clk.restart();

		enemy.update(deltaTime);
		std::cout << enemy.getWorldPos() << "\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	*/

	/*
	TempMap temp_map;

	std::function<bool(sf::Vector2i)> func = [&](sf::Vector2i vec) {
		return temp_map.isWalkable(vec);
		};

	std::vector <sf::Vector2i> walkables;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (temp_map.isWalkable(sf::Vector2i(j, i)))
				walkables.push_back(sf::Vector2i(j, i));
		}
	}

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> permutations;

	for (int i = 0; i < walkables.size(); i++) {
		for (int j = i; j < walkables.size(); j++) {
			permutations.push_back(std::pair<sf::Vector2i, sf::Vector2i>(walkables[i], walkables[j]));
		}
	}
	*/

	/*auto a = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(9, 0), sf::Vector2i(5, 7));
	auto b = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(9, 0), sf::Vector2i(4, 8));
	auto c = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(9, 0), sf::Vector2i(3, 9));
	auto d = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(9, 1), sf::Vector2i(5, 7));
	auto e = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(9, 1), sf::Vector2i(4, 8));
	auto f = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(9, 1), sf::Vector2i(3, 9));
	auto g = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(1, 2), sf::Vector2i(4, 6));
	auto h = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(1, 2), sf::Vector2i(5, 7)); 
	auto i = std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(1, 2), sf::Vector2i(6, 8)); // handle.values() = ... 394 line
	auto temp = std::find(permutations.begin(), permutations.end(), a);
	permutations.erase(temp);
	temp = std::find(permutations.begin(), permutations.end(), b);
	permutations.erase(temp);
	temp = std::find(permutations.begin(), permutations.end(), c);
	permutations.erase(temp);
	temp = std::find(permutations.begin(), permutations.end(), d);
	permutations.erase(temp);
	temp = std::find(permutations.begin(), permutations.end(), e);
	permutations.erase(temp);
	temp = std::find(permutations.begin(), permutations.end(), f);
	permutations.erase(temp);
	temp = std::find(permutations.begin(), permutations.end(), g);
	permutations.erase(temp);
	temp = std::find(permutations.begin(), permutations.end(), h);
	permutations.erase(temp);*/

	//std::vector<sf::Vector2i> path = A_starSearch(func, sf::Vector2i(9, 0), sf::Vector2i(5, 7));

	/*for (auto& permut : permutations) {
		std::cout << permut.first << " and " << permut.second << ": ";
		std::vector<sf::Vector2i> path = A_starSearch(func, permut.first, permut.second);
		std::cout << "done\n";
	}*/

	// Test it for all permutations
	// Create permutations

	/*
		"..........",
		"..........",
		"..........",
		"..#.#.#...",
		".........#",
		".#.####...",
		"......#...",
		"......#...",
		"..........",
		".........."
	*/


	//std::vector<sf::Vector2i> path = A_starSearch(func, sf::Vector2i(2, 0), sf::Vector2i(2, 8));
	/*
	for (auto& e : path)
		temp_map.arr[e.y][e.x] = '+';


	temp_map.arr[1][1] = 's';
	temp_map.arr[6][5] = 'e';


	for (int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++)
			std::cout<<temp_map.arr[i][j];

		std::cout << "\n";
	}
	*/
	GameManager::getManager().update();

    return 0;
}