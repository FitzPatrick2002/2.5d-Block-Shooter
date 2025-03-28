import <iostream>;
import <SFML/Graphics.hpp>;

import GameManager;
import linAlg;
import <functional>;

//import ThreadPool;

// TO DO:

// 1. sf::View -> I can see only a small part of the map. 
// 2. Based on the world position, decide which tiles are to be rendered into the screen (make sure no more than 200 is being rendered)
// 3. Render them in an appropriate order
//      Start naive (each frame check anew from the pool of all tiles which to render
//      Later have the
//      

// 3. 


//OTHER:

//ThreadPool pool;

import <queue>;
import <thread>;
import <mutex>;
import <functional>;
import <vector>;
import <unordered_map>;

import Enemy;

struct Key {
	sf::Vector2i vec;

	Key(sf::Vector2i u) : vec(u) {};

	bool operator==(const Key& other) const {
		return vec.x == other.vec.x and vec.y == other.vec.y;
	}
};

struct KeyHash {
	std::size_t operator()(const Key& k) const {
		return std::hash<int>()(k.vec.x) ^ (std::hash<int>()(k.vec.y) << 1);
	}
};

struct MainQueue {
	std::queue<sf::Vector2i> queue;
	std::unordered_map<Key, sf::Vector2i, KeyHash> check_uniqueness;

	// On each position we store value of the node that was visited beofre this one was visited
	std::vector<sf::Vector2i> previous_nodes; // Instead of a vector use a unordered_map so that we only store the visited stuff, terminate working of the class / functior once we arrive at all destinations
	sf::IntRect bounds;

	sf::Vector2i directions[4];

	std::vector<std::string>* map;

	sf::Vector2i start;
	sf::Vector2i end;

	MainQueue(sf::IntRect bounds, std::vector<std::string>* map, sf::Vector2i start, sf::Vector2i end) {
		this->bounds = bounds;
		previous_nodes.resize(bounds.getSize().x * bounds.getSize().y);

		// Init the matrix with (-1,-1)
		for (auto& e : previous_nodes)
			e = sf::Vector2i(-1, -1);

		directions[0] = sf::Vector2i(0, -1); // North
		directions[1] = sf::Vector2i(-1, 0); // West
		directions[2] = sf::Vector2i(0, 1); // South
		directions[3] = sf::Vector2i(1, 0); // East

		this->map = map;
		this->start = start;
		this->end = end;
	}

	bool isInBounds(sf::Vector2i pos) {
		if (pos.x < bounds.getPosition().x or pos.x >= bounds.getPosition().x + bounds.getSize().x)
			return false;

		if (pos.y < bounds.getPosition().y or pos.y >= bounds.getPosition().y + bounds.getSize().y)
			return false;

		return true;
	}

	// Push node that will be visited in the fiuture
	void push(sf::Vector2i future, sf::Vector2i current_pos) {
		if (check_uniqueness.find(future) == check_uniqueness.end()) {

			check_uniqueness[Key(future)] = future;
			queue.push(future);

			this->markPrevious(future, current_pos);
		}
	}

	void markPrevious(sf::Vector2i marked, sf::Vector2i prev) {

		previous_nodes[marked.y * bounds.getSize().x + marked.x] = prev;
	}

	sf::Vector2i getTop() {
		sf::Vector2i front = queue.front();

		check_uniqueness.extract(Key(front));
		queue.pop();

		return front;
	}

	bool hasPath(sf::Vector2i pos) {

		return not (previous_nodes[pos.y * bounds.getSize().x + pos.x] == sf::Vector2i(-1, -1));

	}

	bool isWalkable(sf::Vector2i pos) {
		return ((*map)[pos.y][pos.x] != '#');
	}

	void addNeighbours(sf::Vector2i current_pos) {
		sf::Vector2i neighbour;
		
		for (int i = 0; i < 4; i++) {
			neighbour = current_pos + directions[i];

			if (this->isInBounds(neighbour)) {
				if ((not this->hasPath(neighbour)) and this->isWalkable(neighbour)) {
					this->push(neighbour, current_pos);
				}
			}
		}
	}

	void begin() {
		
		this->push(start, start);

		fillNeighboursMap();
	}

	void fillNeighboursMap() {
		sf::Vector2i current;

		while (not this->queue.empty()) {
			current = this->getTop();

			this->addNeighbours(current);
		}

		/*
		this->addNeighbours(current);
		if (not this->queue.empty())
			fillNeighboursMap(); // Old recursive
		*/
	}

	void printNeighbours() {

		for (int i = 0; i < bounds.getSize().y; i++) {
			for (int j = 0; j < bounds.getSize().x; j++) {
				std::cout << this->previous_nodes[i * bounds.getSize().x + j] << " ";
			}

			std::cout << "\n";
		}

		std::cout << "\n";
	}

	void showFront() {
		std::vector<std::string> temp = *this->map;

		for (auto& e : check_uniqueness) {
			temp[e.second.y][e.second.x] = 'f';
		}

		for (int i = 0; i < bounds.getSize().y; i++) {
			for (int j = 0; j < bounds.getSize().x; j++) {
				std::cout << temp[i][j] << " ";
			}

			std::cout << "\n";
		}
		std::cout << "\n\n";

	}

	void followPath() {
		std::vector<std::string> temp = *this->map;

		sf::Vector2i current = end;
		while(current != start) {
			temp[current.y][current.x] = 'f';

			current = previous_nodes[current.y * bounds.getSize().x + current.x];
			for (int i = 0; i < bounds.getSize().y; i++) {
				for (int j = 0; j < bounds.getSize().x; j++) {
					std::cout << temp[i][j] << " ";
				}

				std::cout << "\n";
			}
			std::cout << "\n\n";
		}
	}
	
};

// 1. Acquire current position
// 2. Set next position no the path as target
// 3. Find versor from current positon to the destination
// 4. Cast a ray from current position to the target
// 5. After ever yincrementation of radiu check if this tile is walkable
// 6. If it is not, stop casting, push previous target to the shortcut and set the current position = that previous target
// Repeat
std::vector<sf::Vector2f> makeShortuct(std::vector<sf::Vector2i>& path, std::vector<std::string> &map) {
	std::vector<sf::Vector2f> shortcut;
	sf::Vector2f delta(0.5f, 0.5f);

	sf::Vector2f target;
	sf::Vector2f current_position = static_cast<sf::Vector2f>(path[0]);
	sf::Vector2i current_position_discrete = path[0];

	float r = 0;
	float delta_r = 0.1;
	sf::Vector2f versor;
	sf::Vector2f connection;

	current_position = current_position + delta;
	for (int i = 1; i < path.size(); i++) {
		target = static_cast<sf::Vector2f>(path[i]);
		target = target + delta;

		r = 0;
		versor = target - current_position;
		versor = 1 / vectorLength(versor) * versor;

		connection = current_position;

		// Cast the ray until we hit obstacle or arrive at the destination
		while (map[connection.y][connection.x] != '#' and current_position_discrete != path[i]) {
			r += delta_r;
			connection = current_position + r * versor;

		}

		//Hit obstacle, assign the previous element of the path to the shortcut
		if (map[connection.y][connection.x] == '#') {

		}
		else if (current_position_discrete == path[i]) {// We have arrived at the destination, look for the next destination
			// Do nothing I guess, bext iteration will decide
		}
		else {
			std::cerr << "Something went terribly wrong\n";
			return shortcut;
		}

	}

	return shortcut;
}
// 1. Get the node from the queue
// 2. Add its neighbours to the queue (at the same they are bing marked)

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


	std::vector<std::string> map = {
"..........",
"#....####.",
"#.###.....",
"....#.....",
"....#.####",
".##.......",
"...#####..",
"..........",
"..........",
".........."
	};

	sf::IntRect rect(0, 0, 10, 10);

	MainQueue temp(rect, &map, sf::Vector2i(3, 0), sf::Vector2i(9, 7));

	temp.begin();

	temp.printNeighbours();

	temp.followPath();

	//GameManager::getManager().update();

    return 0;
}