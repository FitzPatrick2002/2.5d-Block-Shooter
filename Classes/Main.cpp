import <iostream>;
import <SFML/Graphics.hpp>;

import GameManager;
import linAlg;
import <functional>;

// TO DO:
// 1. Make enemies visible (DONE)
// 2. Store them in a list and update them (use the movement manager) (DONE) -> not list an unordered_map keys - positions
// 3. Detect collisions between bullets and enemies (DONE
// 4. Add AI for enemies -> EnemiesManager -> for now very simple just patrolling, no states
// 5. Make the EnemyManager run on threads / Update enemies using threads -> their update functions may get lengthy?

import <queue>;
import <thread>;
import <mutex>;
import <functional>;
import <vector>;
import <unordered_map>;
import <unordered_set>;

import Enemy;
import <ranges>;


int main()
{
	/*
	Enemy enemy;
	
	enemy.setWorldPos(sf::Vector3f(0, 0, 0));
	enemy.setVelMag(0);
	enemy.setVelVersor(sf::Vector3f(0, 0, 0));

	sf::Clock clk;
	sf::Time deltaTime;

	Start_movement mov(sf::Vector2f(1.0f, 0.0f), 2.0f);
	Check_if_at_destination dest(sf::Vector2f(10.0f, 0.0f));
	
	Stop_movement stop;
	mov.getNewVelocityMagnitude();

	enemy.queueCommand(&mov);
	enemy.queueCommand(&dest);
	enemy.queueCommand(&stop);
	
	while (true) {
		deltaTime = clk.restart();

		enemy.update(deltaTime);
		std::cout << enemy.getWorldPos() << "\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	*/


	GameManager::getManager().update();

    return 0;
}