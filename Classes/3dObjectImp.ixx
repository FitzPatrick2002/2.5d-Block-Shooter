import Object_3d;

import linAlg;
import <vector>;


Object_3d::Object_3d() {
	
	this->poinsNum = 0;
}

Object_3d::~Object_3d() {

}

void Object_3d::reset(sf::PrimitiveType prim, sf::Vector3f wPos, std::vector<sf::Vector3f> wPts) {
	this->sPoints.setPrimitiveType(prim);

	this->wPoints.clear();
	this->wPoints = wPts;

	this->poinsNum = this->wPoints.size();

	this->sPoints.clear();
	this->sPoints.resize(this->poinsNum);

	this->worldPos = wPos;

	sf::Vector3f ptPos;
	sf::Vector2f sPos;
	for (int i = 0; i < this->poinsNum; i++) {
		ptPos = this->worldPos + this->wPoints[i];
		sPos = convertWorldToScreenInTiles(ptPos);
		sPos = convertTilesToPixels(sPos);

		this->sPoints[i].position = sPos;
		this->sPoints[i].color = sf::Color::Green;
	}

}

void Object_3d::setWorldPosition(sf::Vector3f worldPos) {
	this->worldPos = worldPos;

	sf::Vector3f ptPos; // Point in world coordinates
	sf::Vector2f sPos; // Point on the screeen
	for (int i = 0; i < this->poinsNum; i++) {
		ptPos = this->worldPos + this->wPoints[i];
		sPos = convertWorldToScreenInTiles(ptPos);
		sPos = convertTilesToPixels(sPos);

		this->sPoints[i].position = sPos;
		//this->sPoints[i].color = sf::Color::Green;
	}
}

void Object_3d::setColor(sf::Color color) {
	for (int i = 0; i < this->poinsNum; i++)
		this->sPoints[i].color = color;
}

sf::Vector3f Object_3d::getWorldPos() const {
	return this->worldPos;
}

void Object_3d::render(sf::RenderWindow* w) {
	
	w->draw(sPoints);
}

void Object_3d::batchToArray(sf::VertexArray& arr) {
	
	for (int i = 0; i < this->wPoints.size(); i++)
		arr.append(this->sPoints[i]);
}