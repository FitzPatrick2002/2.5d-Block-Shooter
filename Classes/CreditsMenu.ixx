export module CreditsMenu;

import GameState;
import MenuTile;

// In the credits menu credits are displayed. 
// To go back to the main meny player must press 'm' key.

export class CreditsMenu : public GameState {
private:
	MenuTile message_tile; 
	MenuTile top_tile;

	sf::Font tile_font;
	sf::View menuView;

public:

	CreditsMenu(sf::RenderWindow* window);

	~CreditsMenu();

	void handleUserInput();

	void loadFont(std::string file_name);

	void prepareTile();

	void update(sf::Time deltaTime);

	void render();
};