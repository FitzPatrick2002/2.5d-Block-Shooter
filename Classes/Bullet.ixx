export module Bullet;

import Entity;
import Animation;

import MapBox;

export class Bullet : public Entity{
private:
	//pos, sprite, hitbox (from Entity)
	// velMag, velocityVersor

	MapBox bulletModel;

public:

	Bullet(sf::Vector3f pos = sf::Vector3f(0, 0, 0));
	~Bullet();

	void init(sf::Vector3f pos, float angle);

	void init(sf::Vector3f pos, sf::Vector2f mousePos);

	void moveBullet(sf::Time& deltaTime);

	void updatePositions();

	void updateHitbox();

	void update(sf::Time deltaTime);

	void render(sf::RenderWindow* w);

	void setTexture(sf::Texture& t);



};