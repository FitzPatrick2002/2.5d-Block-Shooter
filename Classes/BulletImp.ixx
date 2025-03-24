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

// Old 2d
void Bullet::updateHitbox() {

	sf::Vector2f hitboxSize = sf::Vector2f(sprite.getTextureRect().getSize().x, sprite.getTextureRect().getSize().y);
	this->hitbox.setSize(hitboxSize);
	this->hitbox.setScale(sprite.getScale());

	//std::cout << "roation: " << this->hitbox.getRotation() << "\n";

	this->hitbox.setPosition(this->sprite.getPosition());
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

void Bullet::setTexture(sf::Texture& t) {
	this->sprite.setTexture(t);

	sf::Vector2f size = sf::Vector2f(sprite.getTextureRect().getSize().x, sprite.getTextureRect().getSize().y);
	this->hitbox.setSize(size);

	this->hitbox.setPosition(this->getScreenPosInPixels());
	this->hitbox.setFillColor(sf::Color(0, 0, 255, 150));
}