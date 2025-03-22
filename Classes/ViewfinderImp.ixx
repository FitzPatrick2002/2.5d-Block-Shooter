import Viewfinder;

Viewfinder::Viewfinder() {

}

Viewfinder::~Viewfinder() {

}

void Viewfinder::updateViewfinder(sf::Vector2f src, sf::Vector2f destination) {
	this->sightLine.setPositions(src, destination);
}

/*
void Viewfinder::setPosOfViewFinder(sf::Vector2f initPos, float angle) {
	this->sightLine.setPosition(initPos);
	this->sightLine.setRotation(angle);
}

void Viewfinder::updateSightLineLength(sf::Vector2f initPos) {
	float len = 
}
*/

// For now we don't use it
void Viewfinder::update(sf::Time deltaTime) {


}

void Viewfinder::render(sf::RenderWindow* w) {

	this->sightLine.render(w);
}