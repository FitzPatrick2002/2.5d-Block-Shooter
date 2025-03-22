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

	sf::Sprite sprite; 
	sf::RectangleShape hitbox; // Old deprieciated

	//std::shared_ptr<std::mutex> m_vx;

public:

	Entity(sf::Vector3f initPos = sf::Vector3f(0, 0, 0));

	~Entity();

	virtual void update(sf::Time deltaTime) = 0;

	virtual void render(sf::RenderWindow* w) = 0;

	// Mechanics

	bool checkIfCollidesWith(Entity& e, sf::RenderWindow* w);

	// Setters 

	void setTexture(sf::Texture& t);

	void setVelVersor(sf::Vector3f v);

	void setVelVersor_X(float x);

	void setVelVersor_Y(float y);

	void setVelVersor_Z(float z);

	void setVelMag(float v);

	void setWorldPos(sf::Vector3f p);

	// Refers to sprite and rectangleHitbox
	void setRotation(float angleDeg);

	// Getters

	// Old rectangel hitbox
	sf::RectangleShape getHitbox();

	sf::Vector3f getWorldPos();

	sf::Vector2f getScreenPosInTiles();

    sf::Vector2f getScreenPosInPixels();

	float getVelMag();

	sf::Vector3f getVelVersor();

	// Get the old sprite 
	sf::Sprite getSprite();

};
