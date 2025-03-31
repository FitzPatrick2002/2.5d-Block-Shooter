export module linAlg;

import <SFML/Graphics.hpp>;
import <ranges>;
import <numeric>;
import <queue>;
import <unordered_set>;
import <unordered_map>;
import <memory>;
import <random>;

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