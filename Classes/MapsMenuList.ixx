export module MapsMenuList;

import GameManager;
import GameState;
import MenuTile;
import <SFML/Graphics.hpp>;
import <vector>;
import <filesystem>;
import <regex>;

namespace fs = std::filesystem;

// Comments DONE

// In the maps menu player can choose on which map is to be loaded. 
// Maps are read from .txt files and their names are displayed on tiels, one under another.
// To select a map, player need to click on map.
// If none map is chosen, an empty scene will be loaded (only player model).
// To get back to the main menu, player must choose the map, then player will be automatically redirected to the main menu.

// Maps files should be in directory relative to where the .exe file of the game is located: Game/Maps.
export class MapsMenuList : public GameState {
private:
	sf::View menuView;

	std::vector<MenuTile> options; // Vector with tiles, containing names of the maps.
	sf::Font tiles_font;

	std::vector<std::string> maps_names; // Stores names of the maps.

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

	virtual void update(sf::Time deltaTime); 

	void render();

};