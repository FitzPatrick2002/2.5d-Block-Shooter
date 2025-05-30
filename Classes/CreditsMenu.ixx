export module CreditsMenu;

import GameState;
import MenuTile;

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