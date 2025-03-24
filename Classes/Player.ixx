export module Player;

import Entity;
import Animation;
import linAlg;
import Viewfinder;

import MapBox;

// TO DO:
// Read animation data from .txt file (amount of animations and their lengths in a strip)

export class Player : public Entity{
private:

	// Shooting
	float sightAngle;
	Viewfinder viewfinder;

	// Temporary 3d appearence
	MapBox playerModel;

public:

	Player(sf::Vector3f initPos, sf::Texture *t);

	Player();

	~Player();

	//void getUserInput();

	void movePlayer(sf::Time deltaTime);

	void updateModelPosition();

	//void 

	void updateSightAngle(sf::Vector2f mouseCoords);

	void update(sf::Time deltaTime);

	void render(sf::RenderWindow* w);

	// Setters and getters

	// Returns angle in degrees
	float getSightAngle();

	bool isMoving();
};