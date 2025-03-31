import Bullet;
import <iostream>;
import <array>;

import linAlg;

Bullet::Bullet(sf::Vector3f pos) : Entity(pos){

	//this->sprite.setPosition(this->getScreenPosInPixels());

	//this->bulletModel.init()
}

Bullet::~Bullet() {


}

// Depreciated and not used anymore
void Bullet::init(sf::Vector3f pos, float angle) {
	this->worldPos = pos;
	float v_x = cos(angle);
	float v_y = sin(angle);

	std::cout << "angle bullet:" << angle << "\n";
	this->setVelVersor(sf::Vector3f(v_x, v_y, 0.0f));
	this->setVelMag(5.0f);

	sf::Vector3f dims(0.1f, 0.1f, 0.1f);
	this->bulletModel.init(this->worldPos, dims);
}

// Mouse position should be with respect to the center of the view
void Bullet::init(sf::Vector3f pos, sf::Vector2f mousePos) {
	this->worldPos = pos;
	float v_x = mousePos.x;
	float v_y = mousePos.y;

	float new_v_x = 1.41f / 2.0f * (v_x + v_y);
	float new_v_y = 1.41f / 2.0f * (-v_x + v_y);// Manual rotation by 45 degrees, later make a matrix for that.

	this->setVelVersor(sf::Vector3f(new_v_x, new_v_y, 0.0f));
	this->setVelMag(5.0f);

	sf::Vector3f dims(0.1f, 0.1f, 0.1f);
	this->bulletModel.init(this->worldPos, dims);
}

void Bullet::moveBullet(sf::Time& deltaTime) {
	worldPos = worldPos + velMag * deltaTime.asSeconds() * velVersor;
}

void Bullet::updatePositions() {
	this->bulletModel.setWorldPosition(this->worldPos);
}

void Bullet::update(sf::Time deltaTime) {

	this->moveBullet(deltaTime);

	this->updatePositions();
}

void Bullet::render(sf::RenderWindow* w) {

	this->bulletModel.render(w);
	//w->draw(sprite);
	//w->draw(hitbox);
}