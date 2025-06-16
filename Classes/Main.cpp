import <iostream>;
import <SFML/Graphics.hpp>;

import ThreadPool;
import GameManager;
import linAlg;
import <functional>;

import <vector>;
import <string>;
import <future>;
import <regex>;

// Comments DONE

// IN PROGRESS: 8.

// TO DO:
// 5. Make menu prettier, add background screen, blurred image of the gameplay, move slowly from side to side, fade and then unfade into a different picture.
// 7. Saving system (player hp, position), (enemies, number. how many left, positions)
// 8. Credits menu
// 9. Async for map loading (100x100 takes a lot of time)
// 10. Options menu (bullet speed, player hp, size of tile, etc
// 
// . . . 
// 21. Fix the resizing of the window

// Next time when making a game:
// 1. UML diagram
// 2. Have Managers do the job instead of specific objects
// 3. Updating the positions of objects -> physics, movement, collisions managers
//		Other idea is to have just a function in the main state of the game (in this one 'Combat' state) that will do all that for each type?
//		So, entities like Enemy, Player, Bullet, etc. only store data, they do not operate on it (not extensively)
// 4. Split program into very small subprograms that can be run int the 'cleanroom' of some sort
//		ex. A* takes a function that checks both if tile is wlakable or not
//		ex. Raycasting reaised in the exact same manner -> will work for different maps and condition all it needs is the function
//		ex. collision checking -> more tricky because it is sort of dependent on how we actaully check the collision (container type)
//		ex. AI Hard to generalise -> more like I duhnno, I really dunno...

// What I'm trying to achieve is to be able to add elements to the program in a very simple manner 
// For example when I want a new enemy I just 


// Other notes:
// Simpler fov -> map is always visible just cast rays to enemies only (same as enemies do with the player)

import <queue>;
import <thread>;
import <mutex>;
import <functional>;
import <vector>;
import <unordered_map>;
import <unordered_set>;

import Enemy;
import <ranges>;

/*

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

void func(std::vector<int> &vec, std::string line_part, int pos_begin) {
	std::cout << "pos_begin = " << pos_begin << "\n";
	std::regex reg("(\\d+)\\s+");
	std::smatch m;

	std::sregex_iterator start = std::sregex_iterator(line_part.begin(), line_part.end(), reg);
	std::sregex_iterator end = std::sregex_iterator();

	int j = 0;
	for (auto i = start; i != end; i++) {
		m = *i;
		vec[pos_begin + j] = std::atoi(m[1].str().c_str());
		//std::cout << "pos_begin = " << pos_begin << " j = " << j << "\n";
		j++;
	}

}

*/


int main()
{
	/*
	std::vector<int> vec(100);

	auto binded_func = std::bind(func, std::ref(vec), std::placeholders::_1, std::placeholders::_2);
	
	std::vector<std::string> data = {
		"1 1 0 0 0 1 0 1 0 0",
		"1 0 0 0 0 1 0 1 0 0",
		"1 5 0 0 0 1 0 1 0 0",
		"0 0 0 0 0 0 0 4 0 0",
		"1 1 0 0 0 1 0 1 0 0",
		"1 1 0 0 0 9 0 0 0 0",
		"1 1 0 0 0 1 0 1 0 0",
		"1 1 0 0 0 1 0 1 0 0",
		"1 1 0 0 0 1 0 1 0 0",
		"1 1 0 0 0 1 0 1 0 0"
	};

	std::vector<std::future<void>> async_tasks(10);
	for (int i = 0;  auto & line : data) {
		// Start async task for each line
		async_tasks[i] = std::async(binded_func, line, i * 10);
		i++;
	}

	for (auto& e : async_tasks)
		e.get();

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			std::cout << vec[i * 10 + j] << " ";
		}
		std::cout << "\n";
	}
	
	*/

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
	

	// Start the thread pool.
	ThreadPool::accessPool().start();

	// Run the game
	GameManager::getManager().update();

	// Stop the threads from the threadpool
	ThreadPool::accessPool().shutdown();

    return 0;
}