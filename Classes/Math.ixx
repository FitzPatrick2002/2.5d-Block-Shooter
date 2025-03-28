export module linAlg;

import <SFML/Graphics.hpp>;
import <ranges>;
import <numeric>;

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