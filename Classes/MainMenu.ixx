export module MainMenu;

import GameState;
import <vector>;

// Switch case for checking which tile has been clicked
// External function (like in a specific class for whcih this menu window would be created)
// And in that external function there is a swithc case to wchih we return the cuurenlty selceted option
// And we call a specific function from the external class

export class MainMenu : public GameState {
private:
	// sf::renderWindow* window; // From GameState class

	sf::RectangleShape testRect;

	std::vector<sf::RectangleShape> options;

public:

	MainMenu(sf::RenderWindow* w);
	~MainMenu();

	void prepareMenuOptions();

	void handleUserInput();

	virtual void update(sf::Time deltaTime); // From polymorphism
	void render();
};