export module MainMenu;

import <SFML/Graphics.hpp>;
import MapsMenuList;
import GameState;
import MenuTile;
import <vector>;

// Comments DONE

// Switch case for checking which tile has been clicked.
// External function (like in a specific class for whcih this menu window would be created).
// And in that external function there is a swithc case to wchih we return the cuurenlty selceted option.
// And we call a specific function from the external class.

// In the main menu player chooses the main options.
// Play - play the game.
// Maps - choose the map to play on (must be done before hitting Play).
// Options (settings) - Player can modify the properties of varous objects like enemies, bullets, map, and player object... Options must be visited before playing the game.
// Credits - shows credits. 
// Quit - exit the game.

export class MainMenu : public GameState {
private:
	std::vector<MenuTile> options; // Vector of tiles that will be displayed

	sf::View menuView;
	sf::Font tiles_font;

public:

	MainMenu(sf::RenderWindow* w);
	~MainMenu();

	void resizeView();

	void loadFont(std::string file_name);

	void prepareMenuOptions();

	void handleUserInput();

	void handleMenuInput();

	sf::Vector2f getMousePosition();

	virtual void update(sf::Time deltaTime); // From polymorphism

	void render();
};