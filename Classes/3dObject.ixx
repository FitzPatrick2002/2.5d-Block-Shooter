export module Object_3d;

import <SFML/Graphics.hpp>;
import <vector>;


export class Object_3d {
protected:
	sf::Vector3f worldPos; // Origin point of our shape

	sf::VertexArray sPoints;
	std::vector<sf::Vector3f> wPoints; // They are with respect to the this->worldPos origin point

	int poinsNum;

public:

	Object_3d();
	~Object_3d();

	// Delets the contents of the object and creates all the vertices anew.
	// Calculates the positions on screen and in the world
	void reset(sf::PrimitiveType prim, sf::Vector3f wPos, std::vector<sf::Vector3f> wPts);

	void setColor(sf::Color color);

	// Sets the position in the world measured in tiles
	void setWorldPosition(sf::Vector3f worldPos);

	sf::Vector3f getWorldPos() const;

	void render(sf::RenderWindow* w);
};