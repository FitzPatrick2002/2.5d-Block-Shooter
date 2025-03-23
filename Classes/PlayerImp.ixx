import Player;
import linAlg;

import <iostream>;
import <vector>;

Player::Player(sf::Vector3f initPos, sf::Texture* t) : Entity(initPos) {
	if (t != nullptr) {
		std::vector<int> vec = { 4 }; // Temporary, later read it from a .txt file

		this->sprite.setTexture(*t);
		this->setAnimationData(t, vec, 0.3f);
	}

	this->velMag = 200.0f; 

	sf::Vector3f dims(0.5f, 0.5f, 0.5f);
	this->playerModel.init(initPos, dims);
}


Player::Player() : Entity() {
	this->velMag = 4.0f;
	
	this->hitbox.setSize(sf::Vector2f(30, 50));
	this->hitbox.setFillColor(sf::Color(255, 0, 0, 100));

	this->setWorldPos(sf::Vector3f(0.0f, 0.0f, 0.0f));

	sf::Vector3f dims(0.5f, 0.5f, 0.5f);
	this->playerModel.init(this->worldPos, dims);
	this->playerModel.setOutlineColor(sf::Color::Cyan);
	this->playerModel.setWallsColor(sf::Color::Black);

	this->gameMap = nullptr;
}

Player::~Player() {
	
}

// Update positions of sprite, hitbox, etc based on the current position
void Player::updateScreenPositions() {
	sf::Vector2f scrPos = this->getScreenPosInPixels();
	sf::Vector2f spriteSize = sf::Vector2f(sprite.getTexture()->getSize().x, sprite.getTexture()->getSize().y);
	this->sprite.setPosition(scrPos + (-1.0f) * spriteSize);
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

void Player::updateHitbox() {
	
	sf::Vector2f hitboxSize = sf::Vector2f(sprite.getTextureRect().getSize().x, sprite.getTextureRect().getSize().y);
	this->hitbox.setSize(hitboxSize);
	this->hitbox.setScale(sprite.getScale());

	this->hitbox.setPosition(this->sprite.getPosition());
}

// 1. Check if after moving we will be still on the same tile
// 2. If not check if the hieght difference wouldn't be too big
// 3. If it won't set the flag that movement can occur and 
// 4. Move the player and update his z-world position

void Player::movePlayer(sf::Time deltaTime) {	

	sf::Vector3f anticipatedFarPosition = worldPos + (deltaTime.asSeconds() + 0.1f) * velMag * velVersor;
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
	}
}

void Player::update(sf::Time deltaTime) {

	//this->getUserInput();

	this->movePlayer(deltaTime);
	this->updateScreenPositions();
	this->updateModelPosition();

	this->animation.update(deltaTime);
	this->sprite.setTextureRect(this->animation.uvRect);

	// Update where we are aiming

	//this->updateHitbox();

}

void Player::render(sf::RenderWindow* w) {

	this->playerModel.render(w);

	//w->draw(this->sprite);
	//w->draw(this->hitbox);

	this->viewfinder.render(w);
}


// 1. Set texture and animation data
// 2. 
void Player::setTexture(sf::Texture& texture) {
	std::vector<int> vec = { 4 };

	this->sprite.setTexture(texture);
	this->setAnimationData(&texture, vec, 0.3f);

	this->sprite.setScale(sf::Vector2f(0.8f, 0.8f));
	
}

void Player::setGameMap(GameMap* map) {
	this->gameMap = map;
}

void Player::setAnimationData(sf::Texture* texture, std::vector<int>& imCount, float tbf) {
	this->animation.resetTexture(texture, imCount, tbf);
}

float Player::getSightAngle() {
	return this->sightAngle;
}

bool Player::isMoving() {
	return (vectorLength(this->velVersor) != 0.0f);
}