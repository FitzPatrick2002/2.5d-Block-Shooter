export module Viewfinder;

import Entity;
import Object_3d;
import <SFML/Graphics.hpp>;

import <iostream>;
import linAlg;

struct Line {
	sf::VertexArray vertices;

	Line() : vertices(sf::LineStrip, 2) {
		vertices[0].color = sf::Color::Red;
		vertices[1].color = sf::Color::Red;
	}

	~Line() {

	}

	void setPositions(sf::Vector2f a, sf::Vector2f b) {
		vertices[0] = a;
		vertices[1] = b;
	
	}
	void render(sf::RenderWindow* w) {

		w->draw(vertices);
	}
};

struct CrossHair {
	sf::RectangleShape sideLines[4];
	sf::CircleShape dot;

	CrossHair() {
		dot.setRadius(10.0f);
		dot.setOrigin(20.0f, 20.0f);
		dot.setFillColor(sf::Color(255, 255, 255, 0));
		dot.setOutlineColor(sf::Color::Red);
		dot.setOutlineThickness(2.0f);

	}
};

export class Viewfinder : public Entity{
private:
	Line sightLine;
	CrossHair crossHair;

public:

	Viewfinder();
	~Viewfinder();

	void updateViewfinder(sf::Vector2f src, sf::Vector2f destination);

	//void setPosOfViewFinder(sf::Vector2f initPos, float angle);

	void render(sf::RenderWindow* w);

private:

	void update(sf::Time deltaTime);

};