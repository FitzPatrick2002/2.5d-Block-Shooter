export module GameMap;

import Object_3d;
import MapBox;

import TextureManager;
import linAlg;

import ThreadPool;

import DFSsearch;

import <SFML/Graphics.hpp>;
import <vector>;
import <fstream>;
import <filesystem>;
import <functional>;
import <tuple>;
import <thread>;
import <unordered_map>;

// Storing 2d array in a 1d array:
// [i][j] == [i * height + j]


// TO DO:
// Add to the map .txt file amount of chunks / size of a chunk

export class GameMap {
private:

	// Width and height m(in tiles) in the world coordinate system
	int width;
	int height;

	std::vector<MapBox> ground;
	std::vector<MapBox> groundForDisplay;

	TextureManager* textureManager;

	//ThreadPool mapPool;

	sf::Vector2i chunks_amount;
	sf::Vector2i chunk_size;

public:

	DFSsearch<GameMap> dfs_search;

public:

	GameMap();
	~GameMap();

	void init(int w, int h, TextureManager* tM);

	void initChunks(); // Call it after downloading / generatng map, or if map is loaded from file, just read the value

	// Terrain generation

	void generateMap(); // Random walls geenration

	// Paths for enemies generation



	// FOV and drawing

	void setFOVraycasting(sf::Vector2f player_pos, sf::Vector2f mouse_position);

	void setPlayerFOV(sf::Vector3f playerPos, sf::Vector2f absoluteMousePosition);

	void iterateOverWidth(int y, int width, sf::Vector3f playerPos, sf::Vector2f mousePos);

	void setRenderOrder();

	void render(sf::RenderWindow* w);

	// Improved rendering by batching

	void batchBoxes(sf::VertexArray& lines_arr, sf::VertexArray& quads_array);

	// Checkers

	bool checkIfOnMap(sf::Vector2f pos);
	bool checkIfOnMap(sf::Vector2i pos);

	bool checkIfTileWalkable(sf::Vector2f pos);
	bool checkIfTileWalkable(sf::Vector2i pos);

	// Getters and setters

	int getHeight();

	int getWidth();

	sf::Vector2i getChunksAmount();

	MapBox getTile(int i, int j);

	// Saving to file

	void loadFromFile(std::string file_name);

};
