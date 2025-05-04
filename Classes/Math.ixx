export module linAlg;

import ThreadPool;

import <SFML/Graphics.hpp>;
import <ranges>;
import <numeric>;
import <queue>;
import <unordered_set>;
import <unordered_map>;
import <memory>;
import <random>;
import <map>;
import <set>;
import <iostream>;
import <functional>;
import <iostream>;

class GameMap;

export class CombatMapData {
private:
	sf::Vector2f scrTileSize;
	std::string map_to_load;

public:
	CombatMapData() {
		this->scrTileSize = sf::Vector2f(40.0f, 20.0f);
		this->map_to_load = "";
	}
	~CombatMapData() {

	}

	static CombatMapData& getCombatMapData() {
		static CombatMapData combatMapData;

		return combatMapData;
	}

	std::string getMapToLoad() {
		return this->map_to_load;
	}

	sf::Vector2f getScrTileSize() {
		return this->scrTileSize;
	}

	void setScrTileSize(sf::Vector2f newTileSize) {
		this->scrTileSize = newTileSize;
	}

	void setMapToLoad(std::string str) {
		this->map_to_load = str;
	}

};

export struct Vector2iHash {
	std::size_t operator()(const sf::Vector2i& v) const {
		return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
	}
};

export struct Pair2iHash {
	std::size_t operator()(const std::pair<sf::Vector2i, sf::Vector2i>& p) const {
		std::size_t h1 = Vector2iHash()(p.first);
		std::size_t h2 = Vector2iHash()(p.second);
		return h1 ^ (h2 << 1); // Combine hashes
	}
};

export class UniqueQueue2i {
private:
	std::queue<sf::Vector2i> queue;
	std::unordered_set<sf::Vector2i, Vector2iHash> check_uniqueness;

public:

	UniqueQueue2i() = default;
	~UniqueQueue2i() = default;

	bool push(sf::Vector2i future, sf::Vector2i current_pos) {

		if (check_uniqueness.insert(future).second) {
			queue.push(future);

			return true;
		}

		return false;
	}

	sf::Vector2i getTop() {
		sf::Vector2i front = queue.front();

		queue.pop();
		check_uniqueness.erase(front);

		return front;
	}

	bool empty() {
		return queue.empty();
	}
};

export sf::Vector2f operator+(sf::Vector2f& v1, sf::Vector2f& v2) {
	sf::Vector2f outcome;
	outcome.x = v1.x + v2.x;
	outcome.y = v1.y + v2.y;

	return outcome;
}

export sf::Vector2f operator-(sf::Vector2f& v1, sf::Vector2f& v2) {
	sf::Vector2f outcome;
	outcome.x = v1.x - v2.x;
	outcome.y = v1.y - v2.y;

	return outcome;
}

export sf::Vector2i operator+(sf::Vector2i& v1, sf::Vector2i& v2) {
	sf::Vector2i outcome;
	outcome.x = v1.x + v2.x;
	outcome.y = v1.y + v2.y;

	return outcome;
}

export sf::Vector2i operator-(sf::Vector2i& v1, sf::Vector2i& v2) {
	sf::Vector2i outcome;
	outcome.x = v1.x - v2.x;
	outcome.y = v1.y - v2.y;

	return outcome;
}

export sf::Vector2i  V3_to_2i(sf::Vector3f v3) {
	return sf::Vector2i((int)(v3.x), (int)(v3.y));
}

export sf::Vector2i  V3_to_2i(sf::Vector3i v3) {
	return sf::Vector2i(v3.x, v3.y);
}

export sf::Vector2i v2f_to_v2i(sf::Vector2f v2) {
	return sf::Vector2i((int)(v2.x), (int)(v2.y));
}

export sf::Vector2f v2i_to_v2f(sf::Vector2i v2) {
	return sf::Vector2f((float)(v2.x), (float)(v2.y));
}

/*
export std::ostream& operator<<(std::ostream& o, sf::Vector2f& v) {
	o << "(" << v.x << ", " << v.y << ")";
	return o;
}*/

export std::ostream& operator<<(std::ostream& o, sf::Vector2i v) {
	o << "(" << v.x << ", " << v.y << ")";
	return o;
}

export std::ostream& operator<<(std::ostream& o, sf::Vector2f v) {
	o << "(" << v.x << ", " << v.y << ")";
	return o;
}

export std::ostream& operator<<(std::ostream& o, sf::Vector3f& v) {
	o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return o;
}

export std::ostream& operator<<(std::ostream& o, sf::Vector3f v) {
	o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return o;
}

export std::ostream& operator<<(std::ostream& o, sf::Vector3i v) {
	o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return o;
}

/*
export sf::Vector2f operator*(float& f, sf::Vector2f& v) {
	sf::Vector2f outcome;
	outcome.x = f * v.x;
	outcome.y = f * v.y;

	return outcome;
}*/

export sf::Vector2f operator*(float f, sf::Vector2f& v) {
	sf::Vector2f outcome;
	outcome.x = f * v.x;
	outcome.y = f * v.y;

	return outcome;
}

export float getAngleOnPlane(float x, float y) {
	float a = std::atan2(y, x);

	return a > 0 ? a : 2*3.14+a;
}

export float vectorLength(sf::Vector2f v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

export float vectorLength(sf::Vector3f v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

export float dotProduct(sf::Vector2f v1, sf::Vector2f v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

export float dotProduct(sf::Vector3f v1, sf::Vector3f v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

export sf::Vector2f makeVersor(sf::Vector2f v) {
	sf::Vector2f outcome = v;
	outcome = 1 / vectorLength(outcome) * outcome;

	return outcome;
}

export sf::Vector2f makeVersor(sf::Vector2f pos_1, sf::Vector2f pos_2) {
	sf::Vector2f outcome = pos_2 - pos_1;
	outcome = 1 / vectorLength(outcome) * outcome;

	return outcome;
}

export sf::Vector2f convertWorldToScreenInTiles(sf::Vector3f worldPos) {
	float x = 0.5f * (worldPos.x - worldPos.y);
	float y = 0.5f * (worldPos.x + worldPos.y) - worldPos.z;

	return sf::Vector2f(x, y);
}

export sf::Vector3f convertScreenInTilesToWorld(sf::Vector2f v, float z) {
	float x = v.x + v.y - z;
	float y = -v.x + v.y - z;

	return sf::Vector3f(x, y, z);
}

// Takes position converted from world to screen using tiles unit
// and returns the position on the screen in pixels
export sf::Vector2f convertTilesToPixels(sf::Vector2f tile) {
	sf::Vector2f scrTileSize = CombatMapData::getCombatMapData().getScrTileSize();

	return sf::Vector2f(tile.x * scrTileSize.x, tile.y * scrTileSize.y);
}

export sf::Vector2f convertPixelsToTiles(sf::Vector2f pixels) {
	sf::Vector2f scrTileSize = CombatMapData::getCombatMapData().getScrTileSize();

	return sf::Vector2f(pixels.x / scrTileSize.x, pixels.y / scrTileSize.y);
}

export
template<class T>
float mean(std::vector<T>& elems) {
	T sum = T();

	for (auto& e : elems)
		sum += e;

	return (sum / elems.size());
}

export int get_random_int(int low, int high) {

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(low, high);

	return dist(rng);
}

export float get_random_float(float low, float high) {

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist(low, high);

	return dist(rng);
}

export sf::Vector2i random_V2i(int low_x, int high_x, int low_y, int high_y) {
	sf::Vector2i outcome;

	outcome.x = get_random_int(low_x, high_x);
	outcome.y = get_random_int(low_y, high_y);
	
	return outcome;
}

export sf::Vector2f random_V2f(float low_x, float high_x, float low_y, float high_y) {
	sf::Vector2f outcome;

	outcome.x = get_random_float(low_x, high_x);
	outcome.y = get_random_float(low_y, high_y);

	return outcome;
}

export sf::Vector2f rotateVector(sf::Vector2f vec, float angle) {
	sf::Vector2f outcome;

	outcome.x = cos(angle) * vec.x - sin(angle) * vec.y;
	outcome.y = sin(angle) * vec.x + sin(angle) * vec.y;

	return outcome;
}

/*
struct NodeDataHash {
	size_t operator()(const NodeData& data) const {
		return std::hash<int>()(data.position.x) ^ std::hash<int>()(data.position.y);
	}
};
*/

struct NodeData {
	sf::Vector2i position;
	sf::Vector2i previous;
	int H_cost;
	int G_cost;

	void set(sf::Vector2i pos, sf::Vector2i prev, int H, int G) {
		this->position = pos;
		this->previous = prev;
		this->H_cost = H;
		this->G_cost = G;
	}

	int getTotalCost() const {
		return H_cost + G_cost;
	}
	bool operator>(const NodeData& n) {
		return this->getTotalCost() > n.getTotalCost();
	}
};

struct NodeDataComparator {
	bool operator()(const NodeData& a, const NodeData& b) const {
		if (a.getTotalCost() != b.getTotalCost()) {
			return a.getTotalCost() < b.getTotalCost();
		}
		if(a.G_cost != b.G_cost) {
			return a.G_cost < b.G_cost;
		}
		if (a.H_cost != b.H_cost) {
			return a.H_cost < b.H_cost;
		}
		if (a.position.x != b.position.x) {
			return a.position.x < b.position.x;
		}
		if (a.position.y != b.position.y) {
			return a.position.y < b.position.y;
		}
		
		return false;
	}
};


// Rewrite this so that it uses std::unordered_map 
// Also have a set for storing them in an ordered way. 
// So basically 
struct ExplorationContainer {
	//std::set<NodeData, NodeDataComparator> queue;
	std::unordered_map <sf::Vector2i, NodeData, Vector2iHash> map;
	std::set<NodeData, NodeDataComparator> set; // Actually I will need to store the position in the set. Because 

	// !!!

	// ACTAULLY I CHANGED SET -> MULTISET ? - And now it works? maybe some nodes had the same predecessor, costs and stuff? I dunno... 

	// !!!

	// If element is new, just insert it
	// If it already exist check if the new data is more promising (lower costs)
	// If so, overwrite existing, if not, don't
	void insert(sf::Vector2i position, NodeData& data) {

		if (map.find(position) == map.end()) {
			map[position] = data;

			if (not set.insert(data).second) {
				std::cout << "asdada\n";
			}
		}
		else {
			NodeData old_data = map[position];

			// Insert only if new cost is lower than the current
			// If the 

			if (old_data.getTotalCost() < data.getTotalCost()) {
				return;
			}
			else {
				if (old_data.getTotalCost() == data.getTotalCost()) {
					if (old_data.H_cost < data.H_cost)
						return;
				}

				// Reinsert into set updated value
				auto handle = set.extract(old_data);
				handle.value() = data;
				set.insert(std::move(handle));

				map[position] = data; // Set new data for that bucket on the map
			}

		}
	}

	void erase(sf::Vector2i position) {
		if (map.find(position) != map.end()) {
			NodeData temp = map[position];

			set.erase(temp);
			map.erase(position);
		}
		else {
			std::cerr << "Can't delete smth from the map / queue that doesn't exist in it\n";
		}

	}

	void updateExisting(sf::Vector2i position, NodeData new_data) {

		if (map.find(position) != map.end()) {
			NodeData old_data = map[position];

			auto temp = set.extract(old_data);
			temp.value().position = new_data.position;
			temp.value().G_cost = new_data.G_cost;
			temp.value().H_cost = new_data.H_cost;
			temp.value().previous = new_data.previous;

			set.insert(std::move(temp));
			map[position] = new_data;
		}
		else {
			std::cerr << "Can't update something that doesn't exist in the map\n";
		}

	}

	NodeData getFrontData() {
		if (not set.empty())
			return *set.begin();

		std::cerr << "Can't get front of the set, there is nothing isnide!\n";
		return NodeData();
	}

	sf::Vector2i getFrontPosition() {
		if (not set.empty())
			return set.begin()->position;

		std::cerr << "Can't get front position of the set, there is nothing isnide!\n";
		return sf::Vector2i(0, 0);
	}

};

/*
struct ExplorationContainer {
	std::set<NodeData, NodeDataComparator> queue;

	void insert(NodeData& data) {
		queue.insert(data);
	}

	NodeData find() {

	}

	void updateExisting(NodeData& old_data, NodeData new_data) {
		if (queue.find(old_data) != queue.end()) {
			auto handle = queue.extract(old_data);

			handle.value().previous = new_data.previous;
			handle.value().G_cost = new_data.G_cost;
			handle.value().H_cost = new_data.H_cost;

			queue.insert(std::move(handle));
		}
		else {
			std::cerr << "Cannot update data in the queue (set really), beacuse it sodesnt exist in it\n";
		}
	}

	NodeData erase(NodeData& data) {
		NodeData temp;

		if (queue.find(data) != queue.end()) {
			temp = *queue.find(data);
			queue.erase(data);
		}
		else {
			std::cerr << "Can't delete data that doesn't exist in the queue/set\n";
		}

		return temp;
	}

};
*/

/*struct UniquePriorityQueue {
	std::priority_queue<NodeData> queue;
	std::unordered_set<NodeData> set;

	void add(NodeData& data) {
		if (set.insert(data).second) {
			queue.push(data);
		}
	}

	NodeData getFirst() {
		if (not queue.empty()) {
			NodeData temp = queue.top();
			set.erase(temp);
			queue.pop();

			return temp;
		}
		else {
			return NodeData();
		}
	}

};*/

//bool checkIfTileWalkable(sf::Vector2f pos);

/*
	ERRORS: 
	- Map contains key to element that doesnt exist in the set. 
*/

export std::vector<sf::Vector2i> A_starSearch(std::function<bool(sf::Vector2i)>& isWalkable, sf::Vector2i start, sf::Vector2i end) {
	ExplorationContainer for_exploration;
	std::unordered_map<sf::Vector2i, NodeData, Vector2iHash> already_visited;

	sf::Vector3i debugging[10][10];

	sf::Vector2i directions[8] = {
		{0, -1}, //North
		{1, -1}, //North-East
		{1, 0}, // East
		{1, 1}, // South-East
		{0, 1}, // South
		{-1, 1}, // South-West
		{-1, 0}, // West
		{-1, -1} // North-West
	};

	// Cost to tgetting there from end
	auto calc_H_cost = [&](sf::Vector2i position, sf::Vector2i end) {
		int delta_x = abs(end.x - position.x);
		int delta_y = abs(end.y - position.y);

		int normal_cost = std::min(delta_x, delta_y);
		int diagonal_cost = std::max(delta_x, delta_y) - normal_cost;

		return normal_cost * 10 + diagonal_cost * 14;
		};

	// Cost to getting there from start
	auto calc_G_cost = [&](sf::Vector2i position, sf::Vector2i prev) {
		int dx = abs(prev.x - position.x);
		int dy = abs(prev.y - position.y);

		int cost;
		if (dx != dy)
			cost = 10;
		else
			cost = 14;

		return cost + already_visited[prev].G_cost;
		};

	// 1. Push the start node

	sf::Vector2i current_position;
	NodeData current_data;

	current_position = start;
	current_data.set(start, start, calc_H_cost(start, end), 0);

	for_exploration.insert(current_position, current_data);

	while (current_position != end) {

		// Get the most promising node from the front of the queue. 
		// Delete it from the queue and mark as already explored

		current_position = for_exploration.getFrontPosition();

		current_data = for_exploration.getFrontData();
		//std::cout << "Current position: " << current_position << "\n";
		/*debugging[current_position.y][current_position.x] = sf::Vector3i(current_data.G_cost, current_data.getTotalCost(), current_data.H_cost);

		std::cout << "Current position: " << current_position << "\n";

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				std::cout << debugging[i][j] << " ";
			}
			std::cout << "\n";
		}*/

		for_exploration.erase(current_position);

		already_visited[current_position] = current_data;

		// Push the neighbours of the current node to the exploration queue
		for (int i = 0; i < 8; i++) {
			sf::Vector2i neighbour_position = current_position + directions[i];

			if (isWalkable(neighbour_position) and (already_visited.find(neighbour_position) == already_visited.end())) {
				NodeData neighbour_data;

				neighbour_data.position = neighbour_position;
				neighbour_data.previous = current_position;
				neighbour_data.G_cost = calc_G_cost(neighbour_position, current_position);
				neighbour_data.H_cost = calc_H_cost(neighbour_position, end);

				for_exploration.insert(neighbour_position, neighbour_data);
				
			}
		}

		/*
		std::cout << "map: ";
		for (auto& e : for_exploration.map)
			std::cout << e.first << " ";

		std::cout << "\n\nset: ";

		for (auto& e : for_exploration.set)
			std::cout << e.position << " ";

		std::cout << "\n\n";

		std::cout << "chosen coords: " << start << " " << end << "\n";
		std::cout << "Sizes: " << for_exploration.map.size() << " " << for_exploration.set.size() << "\n";
		
		/*
		for (auto m : for_exploration.map) {
			if (for_exploration.set.find(m.second) == for_exploration.set.end()) {
				std::cerr << "IS in map but not in set: " << m.first << "\n";
			}
		}

		for (auto s : for_exploration.set) {
			if (for_exploration.map.find(s.position) == for_exploration.map.end()) {
				std::cout << "Error at: " << s.position << "\n";// Error at (7,4)
			}
		}*/

		if (for_exploration.map.size() != for_exploration.set.size()) {
			std::cout << "Error here\n";
		}

	}

	std::vector<sf::Vector2i> path;

	sf::Vector2i current = end;
	while (current != start) {
		path.push_back(current);
		current = already_visited[current].previous;
	}

	std::reverse(path.begin(), path.end());

	return path;
}

export sf::Vector2i getRandomWalkable(std::function<bool(sf::Vector2i)> isWalkable, sf::Vector2i x_range, sf::Vector2i y_range) {
	sf::Vector2i pos = random_V2i(x_range.x, x_range.y, y_range.x, y_range.y);

	int i = 0;
	while (not isWalkable(pos) and i < 100) {
		pos = random_V2i(x_range.x, x_range.y, y_range.x, y_range.y);
		i++;
	}

	if (i < 100) {
		return pos;
	}

	std::cerr << "Can't find random walkable position";
}

// Rewritten code with namespaces

namespace Hashing {

	export struct V2iHash {
		size_t operator()(const sf::Vector2i& p1) const {
			return std::hash<int>()(p1.x) ^ std::hash<int>()(p1.y) << 1;
		}
	};

}

namespace Geometry {
	float pi = 3.14159;

	export float normAngle(float angle) {
		float a = angle / (2.0f * pi);
		float b = angle - floor(a) * 2.0f * pi;

		return b < 0 ? 2.0f * pi + b : b;

	}

	export void drawArch_f(std::vector<sf::Vector2i>& points, sf::Vector2i o, int r, float theta, float alpha) {
		float start_angle = normAngle(theta - alpha);
		float end_angle = normAngle(theta + alpha);

		points.clear();
		points.reserve(ceil(8.0f * static_cast<float>(r) * (alpha / pi)));

		if (end_angle < start_angle) {
			end_angle += 2 * pi;
		}

		float step = 0.5f * pi / points.capacity();
		for (float a = start_angle; a < end_angle; a += step) {
			float aa = normAngle(a);
			points.emplace_back((int)ceil(r * cos(aa)) + o.x, (int)ceil((float)r * sin(aa)) + o.y);
		}

		std::vector<sf::Vector2i>::iterator itr = std::unique(points.begin(), points.end());
		points.resize(std::distance(points.begin(), itr));
		points.shrink_to_fit();
	}

	export void drawLineShortRayCasting(std::vector<sf::Vector2i>& points, std::function<bool(int, int)> isWalkable, int max_x, int max_y, int x0, int y0, int x1, int y1, int n) {
		int dx = std::abs(x1 - x0);
		int dy = std::abs(y1 - y0);

		int sx = (x1 > x0) ? 1 : -1;
		int sy = (y1 > y0) ? 1 : -1;

		int err = dx - dy;

		int x = x0;
		int y = y0;

		int steps_total = (dx + dy) / n;
		points.clear();
		points.reserve(steps_total + 1);

		int i = 0;
		while (i < steps_total) {

			//if (x == x1 && y == y1) break;
			

			if (not isWalkable(x, y)) {
				
				if(x > 0 and y > 0 and x < max_x and y < max_y)
					points.emplace_back(sf::Vector2i(x, y));

				points.shrink_to_fit();
				return;
			}

			points.emplace_back(sf::Vector2i(x, y));

			int e2 = 2 * err;
			if (e2 > -dy) {
				err -= dy;
				x += sx;
				i++;
			}
			if (e2 < dx) {
				err += dx;
				y += sy;
				i++;
			}
		}
	}

	export std::unordered_set<sf::Vector2i, Hashing::V2iHash> rayCastingBresenhamParallel(std::function<bool(int, int)> isWalkable, int max_x, int max_y, sf::Vector2i o, float r, float theta, float alpha) {

		std::vector<sf::Vector2i> rim; // We can optimise it after confirming that it wokrs in that way that we don't loop twice over this vector
		drawArch_f(rim, o, r * 2.0f, theta, alpha);

		std::vector<std::vector<sf::Vector2i>> rays(rim.size());
		for (int i = 0; i < rim.size(); i++) {
			sf::Vector2i p = rim[i];

			drawLineShortRayCasting(rays[i], [&](int x, int y) {return isWalkable(x, y); },max_x, max_y, o.x, o.y, p.x, p.y, 2);
		}

		// Wait for all the rays to be evaluated
		while (ThreadPool::accessPool().anyThreadWorking() or ThreadPool::accessPool().getQueueLength() != 0) {}

		// Insert points into the set so that they do not repeat. Though more efficient would probably be drawing the map in here
		// , I mean setting the vector<bool> for drawing the ma, or just drawing the map in here. Probably yes. On the other had we would need a
		// set<> that is ordered.
		// Becuase then we would have to zero it during each iteration I guess? Whatever. Let's put this stuff into a set
		std::unordered_set<sf::Vector2i, Hashing::V2iHash> outcome;
		for (auto& vec : rays)
			outcome.insert(vec.begin(), vec.end());

		return outcome;
	}

}

namespace geom = Geometry;
