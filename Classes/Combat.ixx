export module Combat;

import GameState;

import <SFML/Graphics.hpp>;
import <iostream>;
import <list>;
import <random>;
import <unordered_set>;

import Player;
import TextureManager;
import Bullet;

import GameMap;

struct HashListIterator {
	template<class T>
	std::size_t operator()(const T& it) const {
		return std::hash<const void*>()(std::addressof(*it));
	}
};

export class Combat : public GameState {
private:

	//sf::RenderWindow* window;

	TextureManager textureManager;

	// Objects on the map

	std::list<Bullet> bullets;
	std::unordered_set<std::list<Bullet>::iterator, HashListIterator> bulletsToRemove;

	Player player;
	
	bool finishThreads;
	//std::shared_ptr<std::thread> playerInputThread;
	//std::shared_ptr<std::thread> bulletsCollisionsThread;

	// Temporary testing the 3d objects
	GameMap map;

	sf::Vector2f playerViewSize;
	sf::View playerView;
	float playerViewScrollVal;

public:

	Combat(sf::RenderWindow* win);

	~Combat();

	void initialiseMap();

	void initPlayerView();
	
	void updatePlayerView();

	void updatePlayerFOV();

	void startThreads();

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

	void checkCollisions();

	void removeObjects();

	// Here we put all the stuff that does not regard rendering and graphics
	// User input, Updating positions, physics, etc.
	virtual void update(sf::Time deltaTime);

	// Here we only draw things onto the
	void render();

	void resizeView();

	//  ------------------------------- Setters and getters ----------------------------------
	// They are here only so that I can access varibale sin threads.
	// There probably is some nicer way of handling this stuff

	bool getFinishThreads();

};