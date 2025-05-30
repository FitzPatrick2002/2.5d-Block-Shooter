import Player;
import linAlg;

import <iostream>;
import <vector>;

Player::Player(sf::Vector3f initPos, sf::Texture* t) : Entity(initPos) {
	this->velMag = 200.0f; 

	sf::Vector3f dims(0.5f, 0.5f, 0.5f);
	this->playerModel.init(initPos, dims);
}


Player::Player() : Entity() {
	//this->velMag = 4.0f;
	this->setVelMag(CombatMapData::getCombatMapData().getPlayerSpeed());
	this->hp = CombatMapData::getCombatMapData().getPlayerHp();

	this->setWorldPos(sf::Vector3f(0.0f, 0.0f, 0.0f));

	sf::Vector3f dims(0.5f, 0.5f, 0.5f);
	this->playerModel.init(this->worldPos, dims);
	this->playerModel.setOutlineColor(sf::Color::Cyan);
	this->playerModel.setWallsColor(sf::Color::Black);

}

Player::~Player() {
	
}

void Player::updateModelPosition() {
	this->playerModel.setWorldPosition(this->worldPos);
}

void Player::updateSightAngle(sf::Vector2f mouseCoords) {

	sf::Vector2f temp = convertPixelsToTiles(mouseCoords);
	sf::Vector3f mouseCoords_3d = convertScreenInTilesToWorld(temp, 0.5f);

	float delta_x = mouseCoords_3d.x - this->getWorldPos().x;
	float delta_y = mouseCoords_3d.y - this->getWorldPos().y;

	this->sightAngle = getAngleOnPlane(delta_x, delta_y);

	this->viewfinder.updateViewfinder(this->getScreenPosInPixels(), mouseCoords); // Apparently
}

// 1. Check if after moving we will be still on the same tile
// 2. If not check if the hieght difference wouldn't be too big
// 3. If it won't set the flag that movement can occur and 
// 4. Move the player and update his z-world position

void Player::movePlayer(sf::Time deltaTime) {	

	/*::Vector3f anticipatedFarPosition = worldPos + (deltaTime.asSeconds() + 0.1f) * velMag * velVersor;
	sf::Vector2i current_xy_pos = sf::Vector2i((int)(worldPos.x), (int)(worldPos.y));
	sf::Vector2i anticipated_xy_pos = sf::Vector2i((int)(anticipatedFarPosition.x), (int)(anticipatedFarPosition.y));

	bool updatePos = true;
	if (anticipated_xy_pos != current_xy_pos) {
		float current_z = this->worldPos.z;
		float future_z = this->gameMap->getTile(anticipated_xy_pos.y, anticipated_xy_pos.x).getDimensions().z;

		if(future_z > current_z + 0.3f)
			updatePos = false;

	}
	if (updatePos == true) {
		this->worldPos = worldPos + deltaTime.asSeconds() * velMag * velVersor;
		this->worldPos.z = this->gameMap->getTile(current_xy_pos.y, current_xy_pos.x).getDimensions().z;
		std::cout << "Player z = " << this->worldPos.z << "\n";
	}*/

	//this->worldPos = worldPos + deltaTime.asSeconds() * velMag * velVersor;
	
}

void Player::update(sf::Time deltaTime) {

	//this->movePlayer(deltaTime);
	this->updateModelPosition();

}

void Player::render(sf::RenderWindow* w) {

	this->playerModel.render(w);

	//w->draw(this->sprite);
	//w->draw(this->hitbox);

	this->viewfinder.render(w);
}

float Player::getSightAngle() {
	return this->sightAngle;
}

void Player::decreaseHp(int d) {
	this->hp -= d;
}

int Player::getHp() {
	return this->hp;
}

bool Player::isMoving() {
	return (vectorLength(this->velVersor) != 0.0f);
}