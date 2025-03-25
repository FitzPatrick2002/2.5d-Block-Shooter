export module MainMenu;

import <SFML/Graphics.hpp>;

import MapsMenuList;
import GameState;
import MenuTile;

import <vector>;


// Switch case for checking which tile has been clicked
// External function (like in a specific class for whcih this menu window would be created)
// And in that external function there is a swithc case to wchih we return the cuurenlty selceted option
// And we call a specific function from the external class

export class MainMenu : public GameState {
private:
	// sf::renderWindow* window; // From GameState class
	std::vector<MenuTile> options;

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