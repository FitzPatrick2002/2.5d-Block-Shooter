//#include <mutex>

export module Entity;

import <SFML/Graphics.hpp>;
import linAlg;

import Object_3d;

//import MutexModule;

export class Entity {
protected:

	sf::Vector3f worldPos; // Position specified in tiles

	sf::Vector3f velVersor; // Velocity specified in tiles
	float velMag;

public:

	Entity(sf::Vector3f initPos = sf::Vector3f(0, 0, 0));

	~Entity();

	virtual void update(sf::Time deltaTime) = 0;

	virtual void render(sf::RenderWindow* w) = 0;

	// Mechanics

	//bool checkIfCollidesWith(Entity& e, sf::RenderWindow* w);

	// Setters 

	void setVelVersor(sf::Vector3f v);

	void setVelVersor_X(float x);

	void setVelVersor_Y(float y);

	void setVelVersor_Z(float z);

	void setVelMag(float v);

	void setWorldPos(sf::Vector3f p);

	// Getters

	sf::Vector3f getWorldPos();

	sf::Vector2f getWorld_XY();

	sf::Vector2f getScreenPosInTiles();

    sf::Vector2f getScreenPosInPixels();

	float getVelMag();

	sf::Vector3f getVelVersor();

};
