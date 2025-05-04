//#include <thread>
//#include <mutex>

import Entity;
import linAlg;
import <array>;

import <SFML/Graphics.hpp>;
import <iostream>;

//import MutexModule;

Entity::Entity(sf::Vector3f initPos) : worldPos(initPos) {

	//this->m_vx = std::make_shared<std::mutex>();
}

Entity::~Entity() {

}

// Old collisions
/*
// Bear in mind that it checks if any point of pts_0 is inside pts_1 but not reverse
bool Entity::checkIfCollidesWith(Entity& e, sf::RenderWindow* w) {

	std::array<sf::Vector2f, 4> pts_0;
	std::array<sf::Vector2f, 4> pts_1; 

	sf::Transform tr_0 = this->hitbox.getTransform();
	sf::Transform tr_1 = e.hitbox.getTransform();
	
	// Get the points that are froming the hitboxes
	for (int i = 0; i < 4; i++) {
		pts_0[i] = tr_0.transformPoint(this->hitbox.getPoint(i)); // Get the real position (transforemd of this hitbox)
		
		pts_1[i] = (e.hitbox.getPoint(i)); // Get the non transformed position (so that we can  check as if it was a rectabgle)
	}

	// Check if they collide
	sf::Vector2f left_x = *std::min_element(pts_1.begin(), pts_1.end(), [](sf::Vector2f a, sf::Vector2f b) { // Check where are the bounds of the rectangle
		return a.x < b.x;
		});

	sf::Vector2f right_x = *std::max_element(pts_1.begin(), pts_1.end(), [](sf::Vector2f a, sf::Vector2f b) {
		return a.x < b.x;
		});

	sf::Vector2f top_y = *std::min_element(pts_1.begin(), pts_1.end(), [](sf::Vector2f a, sf::Vector2f b) {
		return a.y < b.y;
		});

	sf::Vector2f bottom_y = *std::max_element(pts_1.begin(), pts_1.end(), [](sf::Vector2f a, sf::Vector2f b) {
		return a.y < b.y;
		});

	for (auto& pOriginal : pts_0) {
		sf::Vector2f p = tr_1.getInverse().transformPoint(pOriginal); // Transform the transformed hitbox to coordinate space where the other one is just a non rotated rectangle
		if (p.x > left_x.x and p.x < right_x.x) {
			if (p.y > top_y.y and p.y < bottom_y.y) // Simple rectangle check if it contains anything
				return true;
			
		}
	}

	return false;

}*/

void Entity::update(sf::Time deltaTime) {

}

void Entity::render(sf::RenderWindow* w) {

}

void Entity::setVelVersor(sf::Vector3f v) {
	float len = vectorLength(v);

	len == 0 ? this->velVersor = v : this->velVersor = (1 / len) * v;
}

//std::mutex m_vx;

void Entity::setVelVersor_X(float x) {
	//std::lock_guard<std::mutex> lock(*m_vx);

	sf::Vector3f v = this->velVersor;
	v.x = x;
	float len = vectorLength(v);

	len == 0 ? this->velVersor = v : this->velVersor = (1 / len) * v;
}

void Entity::setVelVersor_Y(float y) {
	sf::Vector3f v = this->velVersor;
	v.y = y;
	float len = vectorLength(v);

	len == 0 ? this->velVersor = v : this->velVersor = (1 / len) * v;
}

void Entity::setVelVersor_Z(float z) {
	sf::Vector3f v = this->velVersor;
	v.z = z;
	float len = vectorLength(v);

	len == 0 ? this->velVersor = v : this->velVersor = (1 / len) * v;
}

void Entity::setVelMag(float v) {
	this->velMag = v;
}

void Entity::setWorldPos(sf::Vector3f p) {
	this->worldPos = p;
}

sf::Vector3f Entity::getWorldPos() const {

	return this->worldPos;
}

sf::Vector2f Entity::getWorld_XY() const {
	return sf::Vector2f(worldPos.x, worldPos.y);
}

sf::Vector2f Entity::getScreenPosInTiles() {

	return convertWorldToScreenInTiles(this->worldPos);
}

sf::Vector2f Entity::getScreenPosInPixels() {
	
	return convertTilesToPixels(convertWorldToScreenInTiles(this->worldPos));
}

float Entity::getVelMag() {
	
	return this->velMag;
}

sf::Vector3f Entity::getVelVersor() {

	return this->velVersor;
}

sf::Vector2f Entity::getVelVersor_XY() {
	return sf::Vector2f(this->velVersor.x, this->velVersor.y);
}
