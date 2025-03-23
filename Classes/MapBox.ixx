export module MapBox;

import <vector>;
import <SFML/Graphics.hpp>;
import Object_3d;

// MapBox is the basic construction block of the map. 

export class MapBox {
private:

	std::vector<Object_3d> outline;
	std::vector<Object_3d> walls;

	sf::Vector3f dims; // Dimensions

	sf::Color outlineColor;
	sf::Color fillingColor;

	sf::Vector3f worldPos;

public:

	MapBox();
	~MapBox();

	void setWorldPosition(sf::Vector3f newPos);

	sf::Vector3f getWorldPos() const;

	void setOutlineColor(sf::Color color);

	void setWallsColor(sf::Color color, int i = -1);

	void init(sf::Vector3f wPos, sf::Vector3f d = sf::Vector3f(1.0f, 1.0f, 1.0f));

	void render(sf::RenderWindow* w);

	sf::Vector3f getDimensions();

}; 