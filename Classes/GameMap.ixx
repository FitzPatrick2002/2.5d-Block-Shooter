export module GameMap;

import Object_3d;
import MapBox;
import TextureManager;
import linAlg;
import ThreadPool;
import Entity;

import <unordered_set>;
import <SFML/Graphics.hpp>;
import <vector>;
import <fstream>;
import <filesystem>;
import <functional>;
import <tuple>;
import <thread>;
import <unordered_map>;
import <set>;
import <functional>;
import <algorithm>;

// Storing 2d array in a 1d array:
// [i][j] == [i * height + j]

// GameMap class stores the map, that is used in the Combat state. 
// It is composed of MapBoxes which are stored in a 1d vector, which we treat as a 2d array.


export class GameMap {
private:

	// Width and height m(in tiles) in the world coordinate system
	int width;
	int height;

	std::vector<MapBox> ground;
	std::vector<MapBox> groundForDisplay;
	std::unordered_set<sf::Vector2i, Hashing::V2iHash> ground_to_display_s;

	TextureManager* textureManager;

	sf::Vector2i chunks_amount;
	sf::Vector2i chunk_size;

public:

	GameMap();
	~GameMap();

	void init(int w, int h, TextureManager* tM);

	void initChunks(); // Call it after downloading / generatng map, or if map is loaded from file, just read the value

	// Terrain generation

	void generateMap(); // Random walls geenration

	// Paths for enemies generation



	// FOV and drawing

	//void setFOVraycasting(sf::Vector2f player_pos, sf::Vector2f mouse_position);

	void setPlayerFOV(sf::Vector3f playerPos, sf::Vector2f absoluteMousePosition);

	//void iterateOverWidth(int y, int width, sf::Vector3f playerPos, sf::Vector2f mousePos);

	void setRenderOrder(std::vector<Entity*>& elems_to_draw);

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

	MapBox& getTile(int x, int y);

	// Saving to file

	void loadFromFile(std::string file_name);
	

};
