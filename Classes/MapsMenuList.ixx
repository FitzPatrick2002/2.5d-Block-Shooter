export module MapsMenuList;

import GameManager;
import GameState;
import MenuTile;
import <SFML/Graphics.hpp>;
import <vector>;

import <filesystem>;
import <regex>;

namespace fs = std::filesystem;

export class MapsMenuList : public GameState {
private:
	sf::View menuView;

	std::vector<MenuTile> options;
	sf::Font tiles_font;

	std::vector<std::string> maps_names;

public:

	MapsMenuList(sf::RenderWindow* win);
	~MapsMenuList();

	void readMapsFromDirectory();

	void resizeView();

	void loadFont(std::string file_name);

	void prepareMenuOptions();

	void handleUserInput();

	void handleMenuInput();

	sf::Vector2f getMousePosition();

	virtual void update(sf::Time deltaTime); // From polymorphism

	void render();

};