export module DFSsearch;

import linAlg;

import <SFML/Graphics.hpp>;
import <unordered_map>;
import <algorithm>;

export template <class T>
class DFSsearch {
private:
	UniqueQueue2i queue;
	std::unordered_map<sf::Vector2i, sf::Vector2i, Vector2iHash> prev_nodes;

	sf::Vector2i directions[4];

	sf::IntRect bounds;
	T* map;

	//sf::Vector2i start;

	//std::vector<sf::Vector2i> ends;

	//std::vector<std::list<sf::Vector2i>> paths;

public:

	DFSsearch() {
		directions[0] = sf::Vector2i(0, -1);
		directions[1] = sf::Vector2i(-1, 0);
		directions[2] = sf::Vector2i(0, 1);
		directions[3] = sf::Vector2i(1, 0);
	}

	~DFSsearch() {

	}

	void setMap(T* m) {
		this->map = m;
	}

	void setBounds(sf::IntRect b) {
		this->bounds = b;
	}

	void operator()(sf::Vector2i start, sf::Vector2i end, std::vector<sf::Vector2i>& path) {
		this->queue.push(start, start);

		auto addNeighbours = [&, this](sf::Vector2i current_pos) {
			sf::Vector2i neighbour;

			for (int i = 0; i < 4; i++) {
				neighbour = current_pos + directions[i];

				if (this->isInBounds(neighbour)) {
					if ((not this->hasPath(neighbour)) and this->isWalkable(neighbour)) {
						this->pushToQueue(neighbour, current_pos);

						if (neighbour == end)
							return;
					}
				}
			}
			};

		auto calcPath = [&, this]() {

			sf::Vector2i current = end;
			while (current != start) {
				path.push_back(current);

				current = prev_nodes[current];//previous_nodes[current.y * bounds.getSize().x + current.x];
				//std::cout << "current: " << current << "\n";
			}

			std::reverse(path.begin(), path.end());
		};

		// Perform the main algorithm
		sf::Vector2i current;
		while (not this->queue.empty()) {
			current = this->queue.getTop();

			addNeighbours(current);
		}

		
		calcPath(); // Calculate all the paths
	}

private:

	bool isInBounds(sf::Vector2i pos) {

		if (pos.x < bounds.getPosition().x or pos.x >= bounds.getPosition().x + bounds.getSize().x)
			return false;

		if (pos.y < bounds.getPosition().y or pos.y >= bounds.getPosition().y + bounds.getSize().y)
			return false;

		return true;
	}

	void pushToQueue(sf::Vector2i future, sf::Vector2i current) {

		this->queue.push(future, current);
		this->markPrevious(future, current);
	}

	void markPrevious(sf::Vector2i marked, sf::Vector2i prev) {

		prev_nodes[marked] = prev;
	}

	bool hasPath(sf::Vector2i pos) {

		return (prev_nodes.find(pos) != prev_nodes.end());
	}

	bool isWalkable(sf::Vector2i pos) {

		return map->checkIfTileWalkable(pos);
	}

	/*
	void addNeighbours(sf::Vector2i current_pos) {
		sf::Vector2i neighbour;

		for (int i = 0; i < 4; i++) {
			neighbour = current_pos + directions[i];

			if (this->isInBounds(neighbour)) {
				if ((not this->hasPath(neighbour)) and this->isWalkable(neighbour)) {
					this->pushToQueue(neighbour, current_pos);

					//this->showFront();

					// If we added neighbours for this node, check if it wasn't an ending node.
					// If it was, increment the endings counter -> how many paths have been founds already
					// If all, set the global flag and terminate program
					if (std::find(ends.begin(), ends.end(), neighbour) != ends.end()) {
						current_end++;

						if (current_end == ends.size()) {
							end_algorithm = true;
							break;
						}
					}

				}
			}
		}
	}
	*/

	/*void calcPaths() {
		paths.resize(ends.size());

		for (int j = 0; j < ends.size(); j++) {
			sf::Vector2i current = ends[j];

			while (current != start) {
				paths[j].push_front(current);

				current = prev_nodes[current];//previous_nodes[current.y * bounds.getSize().x + current.x];
				//std::cout << "current: " << current << "\n";
			}
		}
	}*/
};