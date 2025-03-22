import MapBox;
import <iostream>;

MapBox::MapBox() {

}

MapBox::~MapBox() {

}

void MapBox::setWorldPosition(sf::Vector3f newPos) {
	this->worldPos = newPos;
	
	for(auto &e : this->walls)
		e.setWorldPosition(newPos);

	for (auto& e : this->outline)
		e.setWorldPosition(newPos);
}

sf::Vector3f MapBox::getWorldPos() const {
	return this->worldPos;
}

void MapBox::init(sf::Vector3f wPos, sf::Vector3f d) {
	this->worldPos = wPos;

	this->dims = d;

	outline.resize(6);

	std::vector<sf::Vector3f> outlineQuad = {
		{0.0f, 0.0f, 0.0f},
		{d.x, 0.0f, 0.0f},
		{d.x, d.y, 0.0f},
		{0.0f, d.y, 0.0f},
		{0.0f, 0.0f, 0.0f},

	};
	outline[0].reset(sf::LinesStrip, this->worldPos, outlineQuad);

	for (auto& e : outlineQuad)
		e.z = d.z;
	outline[1].reset(sf::LinesStrip, this->worldPos, outlineQuad);

	outlineQuad.clear();

	outlineQuad.resize(2);

	outlineQuad[0] = sf::Vector3f(0.0f, 0.0f, 0.0f);
	outlineQuad[1] = sf::Vector3f(0.0f, 0.0f, d.z);
	outline[2].reset(sf::LinesStrip, this->worldPos, outlineQuad);

	outlineQuad[0] = sf::Vector3f(d.x, 0.0f, 0.0f);
	outlineQuad[1] = sf::Vector3f(d.x, 0.0f, d.z);
	outline[3].reset(sf::LinesStrip, this->worldPos, outlineQuad);

	outlineQuad[0] = sf::Vector3f(0.0f, d.y, 0.0f);
	outlineQuad[1] = sf::Vector3f(0.0f, d.y, d.z);
	outline[4].reset(sf::LinesStrip, this->worldPos, outlineQuad);

	outlineQuad[0] = sf::Vector3f(d.x, d.y, 0.0f);
	outlineQuad[1] = sf::Vector3f(d.x, d.y, d.z);
	outline[5].reset(sf::LinesStrip, this->worldPos, outlineQuad);

	this->walls.resize(3);

	std::vector<sf::Vector3f> fillingPts;
	fillingPts.resize(4);

	fillingPts[0] = { 0.0f, d.y, 0.0f }; // Left
	fillingPts[1] = { 0.0f, d.y, d.z };
	fillingPts[2] = { d.x, d.y, d.z };
	fillingPts[3] = { d.x, d.y, 0.0f };

	for (auto& e : fillingPts) {
		e.x *= 0.99f;
		//e.y *= 0.95f;
		e.z *= 0.99f;
	}

	this->walls[0].reset(sf::Quads, this->worldPos, fillingPts);
	
	fillingPts[0] = {0.0f, d.y, d.z }; // Top
	fillingPts[1] = { d.x, d.y, d.z };
	fillingPts[2] = { d.x, 0.0f, d.z };
	fillingPts[3] = { 0.0f, 0.0f, d.z };

	for (auto& e : fillingPts) {
		e.x *= 0.99f;
		e.y *= 0.99f;
		//e.z *= 0.99f;
	}

	this->walls[1].reset(sf::Quads, this->worldPos, fillingPts);
	
	fillingPts[0] = { d.x, d.y, d.z }; // Right
	fillingPts[1] = { d.x, 0.0f, d.z };
	fillingPts[2] = { d.x, 0.0f, 0.0f };
	fillingPts[3] = { d.x, d.y, 0.0f };

	for (auto& e : fillingPts) {
		//e.x *= 0.99f;
		e.y *= 0.99f;
		e.z *= 0.99f;
	}

	this->walls[2].reset(sf::Quads, this->worldPos, fillingPts);
	
	
}

void MapBox::setOutlineColor(sf::Color color) {
	for (auto& e : this->outline)
		e.setColor(color);
}

void MapBox::setWallsColor(sf::Color color, int i) {

	if (i == -1)
		for (auto& e : this->walls)
			e.setColor(color);
	else {
		if (i >= 0 and i <= 2)
			this->walls[i].setColor(color);
		else
			std::cerr << "There is only 3 three walls than can be painted on a 3d box\n";
	}
}

void MapBox::render(sf::RenderWindow* w) {

	for (auto e : this->outline)
		e.render(w);

	for(auto& e : this->walls)
		e.render(w);
}