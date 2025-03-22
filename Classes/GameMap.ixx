export module GameMap;

import MapTile;
import Object_3d;
import MapBox;

import TextureManager;
import linAlg;

import ThreadPool;

import <SFML/Graphics.hpp>;
import <vector>;
import <fstream>;
import <filesystem>;

import <thread>;

// Storing 2d array in a 1d array:
// [i][j] == [i * height + j]

export class GameMap {
private:

	// Width and height m(in tiles) in the world coordinate system
	int width;
	int height;

	std::vector<MapBox> ground;
	std::vector<MapBox> groundForDisplay;

	TextureManager* textureManager;

	//std::vector<std::thread*> threadPool;

	//Delte this for schecking where (0,0,0) is
	MapBox temp;

	ThreadPool mapPool;

public:

	GameMap();
	~GameMap();

	void init(int w, int h, TextureManager* tM);

	void generateMap();

	void setPlayerFOV(sf::Vector3f playerPos, sf::Vector2f absoluteMousePosition);

	void iterateOverWidth(int y, int width, sf::Vector3f playerPos, sf::Vector2f mousePos);

	void setRenderOrder();

	void render(sf::RenderWindow* w);

};