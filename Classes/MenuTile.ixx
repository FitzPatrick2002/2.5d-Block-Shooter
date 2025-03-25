export module MenuTile;

import <SFML/Graphics.hpp>;

export struct MenuTile : public sf::RectangleShape {
public:

	sf::Text text;

public:

	MenuTile();
	~MenuTile();

	void init(sf::Vector2f pos, sf::Vector2f size);

	bool containsMouse(sf::Vector2f mouse_position);

	void initText(sf::Font& f, std::string str);

	void setText(std::string str);

	void setPos(sf::Vector2f pos);

};