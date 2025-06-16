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
import Bullet;
import Enemy;
import ThreadPool;
import EnemiesManager;
import GameMap;
import MovementManager;
import linAlg;

// Comments DONE


// Hashing function that allows to store the std::list<Bullet>::iterator in an std::unordered_map.
struct HashListIterator {
	template<class T>
	std::size_t operator()(const T& it) const {
		return std::hash<const void*>()(std::addressof(*it));
	}
};

// Combat is the main class that goevrns all logic and rendering during gameplay. 
// Once the map is loaded, enemies are spawned and player is able to control the character.
// Center of the screen is always fixed on the player figure. 
// Player can zoom in and out by srcolling. 
// If player scrolls too much, the view is inverted, whole map is displayed upside down
// and player can look from the other side of the map (180 degrees turn). This is a feature not a bug. Leave me alone already.
// Player can move on the 'wsad' keys and shoot with the left mouse button.

// Each enemy has his own AI steering him. 
// Enemies will patrol the area of the map in search for player. If player is close enough to the enemy and there is no obstacle
// on the line of sight between enemy and the player, enemy will enter combat mode and will start shooting at the player. 



export class Combat : public GameState {
private:

	// Objects on the map
	std::list<Bullet> bullets; // Bullets are frequently added / deleted, so they are stored in a list.
	// Unordered_set which stores iterators to the bullets in the bullets list which are to be removed.
	// All bullets listed in it are removed in a single function call.
	std::unordered_set<std::list<Bullet>::iterator, HashListIterator> bulletsToRemove;

	// Enemies are stored in a hash map, so that they can be accessed by their position. 
	// This makes collision handling less computation heavy.
	// Whenever enemy changes tile on which it is standing, this map is updated.
	std::unordered_multimap<sf::Vector2i, Enemy, Vector2iHash> enemies; 
	EnemiesManager enemies_manager; // Handles the enemies movement and AI. 

	Player player; // Playe is the main unit, steered by the user. 

	// Map is made of cuboids. Flat tiles are cuboids with height 0.
	// It handles collisions with map elements and takes part in calculating the player FOV.
	GameMap map;

	// This vector stores objects visible to player (tiles, enemies, bullets, etc.).
	std::vector <Entity*> displayed_objects;

	// Lines array:
	sf::VertexArray lines_buffer; // Unused
	sf::VertexArray quads_buffer; // Unused

	// View handling. Player can zoom in and out by scrolling.
	sf::Vector2f playerViewSize; // Size of the view (how much of the scene will be showed in the window).
	sf::View playerView; // View bound to the player.
	float playerViewScrollVal; // Determines how close to the player the camera is located. playerViewSize is multiplied by it in order to determine how much of the scene is visible.

public:

	Combat(sf::RenderWindow* win);

	~Combat();

	void initialiseMap();

	void initPlayerView();
	
	void initEnemies();

	void updatePlayerView();

	void updatePlayerFOV();

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

	// It updates the current "hitboxes" in the unordered_map enemies
	void updateEnemies(sf::Time deltaTime);

	void updateEnemySpawning();

	// Here we put all the stuff that does not regard rendering and graphics
	// User input, Updating positions, physics, etc.
	virtual void update(sf::Time deltaTime);

	// Here we only draw things onto the
	void render();

	void resizeView();

	void endCondition();
};