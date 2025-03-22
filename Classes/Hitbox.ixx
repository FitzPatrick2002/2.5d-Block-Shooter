export module HitboxClass;

import <SFML/Graphics.hpp>;

import Entity;

export class Hitbox : public Entity {
private:

	// pos, sprite
	sf::RectangleShape box; // Box that serves as hitbox

	Entity* parent;

public:

	Hitbox() {

	}

	~Hitbox() {

	}

	void updatePosition() {

		
	}

	void checkCollision(Entity& ent);

	void update(sf::Time deltaTime) {

		this->updatePosition();
	}

	void render(sf::RenderWindow* w) {

	}

};