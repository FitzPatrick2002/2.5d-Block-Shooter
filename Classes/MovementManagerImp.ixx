import MovementManager;
import <iostream>;

MovementManager::MovementManager() {
	this->gameMap = nullptr;
}

MovementManager::~MovementManager() {

}

MovementManager& MovementManager::getManager() {
	static MovementManager manager;

	return manager;
}

void MovementManager::setManagerData(GameMap* gm) {
	this->gameMap = gm;
}

void MovementManager::removeData() {
	this->gameMap = nullptr;
}

// Make sure player can't leave the map 
// Private member
bool MovementManager::playerOutsideMap(Player& p, sf::Time deltaTime) {
	sf::Vector3f playerPos = p.getWorldPos();
	sf::Vector3f playerVelocityVersor = p.getVelVersor();
	float playerVelMag = p.getVelMag();

	sf::Vector3f anticipatedFarPosition = playerPos + (deltaTime.asSeconds() + 0.1f) * playerVelMag * playerVelocityVersor;

	sf::Vector2i current_xy_pos = sf::Vector2i((int)(playerPos.x), (int)(playerPos.y));
	sf::Vector2i anticipated_xy_pos = sf::Vector2i((int)(anticipatedFarPosition.x), (int)(anticipatedFarPosition.y));

	int h = this->gameMap->getHeight();
	int w = this->gameMap->getWidth();

	if (anticipated_xy_pos.x < 0 or anticipated_xy_pos.x >= w)
		return true;

	if (anticipated_xy_pos.y < 0 or anticipated_xy_pos.y >= h)
		return true;

	return false;
}

void MovementManager::movePlayer(Player& p, sf::Time deltaTime) {
	sf::Vector3f playerPos = p.getWorldPos();
	sf::Vector3f playerVelocityVersor = p.getVelVersor();
	float playerVelMag = p.getVelMag();

	sf::Vector3f anticipatedFarPosition = playerPos + (deltaTime.asSeconds() + 0.1f) * playerVelMag * playerVelocityVersor;

	sf::Vector2i current_xy_pos = sf::Vector2i((int)(playerPos.x), (int)(playerPos.y));
	sf::Vector2i anticipated_xy_pos = sf::Vector2i((int)(anticipatedFarPosition.x), (int)(anticipatedFarPosition.y));

	bool updatePos = true;
	float current_z;
	float future_z = playerPos.z;
	if (not this->playerOutsideMap(p, deltaTime)) {
		if (anticipated_xy_pos != current_xy_pos) {
			current_z = playerPos.z;
			future_z = this->gameMap->getTile(anticipated_xy_pos.x, anticipated_xy_pos.y).getDimensions().z;


			if (future_z > current_z + 2.0f) {
				updatePos = false;
				std::cout << "Future z: " << future_z << "\n";
				std::cout << "current z: " << playerPos.z << "\n";

			}

		}

		if (updatePos == true) {
			playerPos = playerPos + deltaTime.asSeconds() * playerVelMag * playerVelocityVersor;
			playerPos.z = this->gameMap->getTile(anticipated_xy_pos.x, anticipated_xy_pos.y).getDimensions().z;

			p.setWorldPos(playerPos);
		}
	}
}