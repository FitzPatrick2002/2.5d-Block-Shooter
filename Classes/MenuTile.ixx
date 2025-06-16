export module MenuTile;

import <SFML/Graphics.hpp>;

// Comments DONE

// MenuTile is a tile displayed in all menus, like Main menu, maps menu, etc.
// It is basically a rectangel that contains text.
// It can check if mouse hovers over it.

export struct MenuTile : public sf::RectangleShape {
private:
	sf::Text text; // Text stored by the tile.

public:

	MenuTile();
	~MenuTile();

	void init(sf::Vector2f pos, sf::Vector2f size);

	bool containsMouse(sf::Vector2f mouse_position);

	void initText(sf::Font& f, std::string str);

	void setText(std::string str);

	sf::Text getText();

	void setPos(sf::Vector2f pos);

};