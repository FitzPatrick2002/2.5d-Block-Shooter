export module MapBox;

import <vector>;
import <SFML/Graphics.hpp>;
import Object_3d;
import Entity;

// MapBox is the basic construction block of the map. 

export class MapBox : public Entity {
private:

	std::vector<Object_3d> outline;
	std::vector<Object_3d> walls;

	sf::Vector3f dims; // Dimensions

	sf::Color outlineColor;
	sf::Color fillingColor;

	sf::Vector3f worldPos;

	void setWorldPos(sf::Vector3f p) {}

public:

	MapBox();
	~MapBox();

	void init(sf::Vector3f wPos, sf::Vector3f d = sf::Vector3f(1.0f, 1.0f, 1.0f));

	// Getters and setters

	void setWorldPosition(sf::Vector3f newPos);

	sf::Vector3f getWorldPos() const;

	sf::Vector3f getDimensions();

	void setOutlineColor(sf::Color color);

	void setWallsColor(sf::Color color, int i = -1);

	// Other

	void render(sf::RenderWindow* w);

	void batchLines(sf::VertexArray& arr);
	void batchQuads(sf::VertexArray& arr);

}; 