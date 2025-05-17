export module Combat;

import GameState;

import <SFML/Graphics.hpp>;
import <iostream>;
import <list>;
import <random>;
import <unordered_set>;
import <unordered_map>;


import Entity;
import Player;
import TextureManager;
import Bullet;
import Enemy;
import ThreadPool;

import EnemiesManager;

import GameMap;
import MovementManager;

import linAlg;


struct HashListIterator {
	template<class T>
	std::size_t operator()(const T& it) const {
		return std::hash<const void*>()(std::addressof(*it));
	}
};

export class Combat : public GameState {
private:

	TextureManager textureManager;

	// Objects on the map

	std::list<Bullet> bullets;
	std::unordered_set<std::list<Bullet>::iterator, HashListIterator> bulletsToRemove;

	//std::unordered_map<sf::Vector2i, Enemy, Vector2iHash> enemies;
	std::unordered_multimap<sf::Vector2i, Enemy, Vector2iHash> enemies;
	EnemiesManager enemies_manager;

	Player player;

	// Temporary testing the 3d objects
	GameMap map;

	// FOV
	std::vector <Entity*> displayed_objects;

	// Lines array:
	sf::VertexArray lines_buffer;
	sf::VertexArray quads_buffer;

	sf::Vector2f playerViewSize;
	sf::View playerView;
	float playerViewScrollVal;

	// Multithreading

public:

	Combat(sf::RenderWindow* win);

	~Combat();

	void initialiseMap();

	void initPlayerView();
	
	void initEnemies();

	void updatePlayerView();

	void updatePlayerFOV();

	void createWindow();

	// In the future make them load from a list 
	// that would be in a .txt or smth.
	void loadTextures();

	// Evoked in handleEvents
	void handleUserInput();

	sf::Vector2f getMousePos();

	sf::Vector2f getRawMousePosRelCenterNormalized();

	// Creates a new bullet and puts it in the bullets list
	void createPlayersBullet();

	// Events Handling, user input
	void handleEvents();

	void addBulletForRemoval(std::list<Bullet>::iterator &itr);

	void checkBulletsCollisions();

	void checkEnemiesCollisions();

	void checkCollisions();

	void removeObjects();

	// It updates the current "hitboxes" in the unordered_map enemies
	void updateEnemies(sf::Time deltaTime);

	// Here we put all the stuff that does not regard rendering and graphics
	// User input, Updating positions, physics, etc.
	virtual void update(sf::Time deltaTime);

	// Here we only draw things onto the
	void render();

	void resizeView();

	//  ------------------------------- Setters and getters ----------------------------------
	// They are here only so that I can access varibale sin threads.
	// There probably is some nicer way of handling this stuff

};