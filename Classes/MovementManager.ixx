export module MovementManager;

import <SFML/Graphics.hpp>;

import Player;
import GameMap;

export class MovementManager {
private:
	GameMap* gameMap;

private:

	bool playerOutsideMap(Player& p, sf::Time deltaTime);

public:

	MovementManager();
	~MovementManager();

	void setManagerData(GameMap* gm);

	static MovementManager& getManager();

	// After deleting the Combat state set all pointers to nullptr
	void removeData();

	void movePlayer(Player &p, sf::Time deltaTime);
};