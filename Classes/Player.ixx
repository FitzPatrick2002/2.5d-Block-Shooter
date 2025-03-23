export module Player;

import Entity;
import Animation;
import linAlg;
import Viewfinder;

import MapBox;
import GameMap;

// TO DO:
// Read animation data from .txt file (amount of animations and their lengths in a strip)

export class Player : public Entity{
private:

	// pos, sprite -> from inheritence
	// 
	// Animations
	Animation animation;

	// Shooting
	float sightAngle;
	Viewfinder viewfinder;

	// Temporary 3d appearence
	MapBox playerModel;

	// The game map :/	
	GameMap* gameMap;

public:

	Player(sf::Vector3f initPos, sf::Texture *t);

	Player();

	~Player();

	//void getUserInput();

	void movePlayer(sf::Time deltaTime);

	void updateModelPosition();

	// Update positions of sprite, hitbox, etc based on the current position
	void updateScreenPositions();

	//void 

	void updateSightAngle(sf::Vector2f mouseCoords);

	void updateHitbox();

	void update(sf::Time deltaTime);

	void render(sf::RenderWindow* w);

	// Setters and getters

	// Texture is a single file with all possible animations
	void setTexture(sf::Texture& texture);

	void setGameMap(GameMap* map);

	void setAnimationData(sf::Texture* texture, std::vector<int>& imCount, float tbf);

	// Returns angle in degrees
	float getSightAngle();

	bool isMoving();
};