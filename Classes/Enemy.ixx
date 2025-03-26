export module Enemy;

import EnemyCommands;
import Entity;
import MapBox;

import linAlg;

import <queue>;
import <functional>;
import <utility>;
import <iostream>;

import <SFML/Graphics.hpp>;

export class Enemy : public Entity {
private:
	MapBox model_3d;

	std::queue<std::shared_ptr<BasicCommand>> commands_queue;
	std::shared_ptr<BasicCommand> current_command;

public:

	Enemy();
	~Enemy();

	void updatePosition(sf::Time deltaTime);

	void update(sf::Time deltaTime);

	void performCommand();

	void queueCommand(BasicCommand* command);

	void render(sf::RenderWindow* win);

};