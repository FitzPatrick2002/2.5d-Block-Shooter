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

class Player;

export enum class EnemyState
{
	Patrol,
	Combat
};

export class Enemy : public Entity {
private:
	MapBox model_3d;

	EnemyState current_status;
	float view_range;

	std::queue<std::shared_ptr<BasicCommand>> commands_queue;
	std::shared_ptr<BasicCommand> current_command;

private:

	void updatePosition(sf::Time deltaTime);

	void performCommand();

public:

	Enemy();
	~Enemy();

	void init(sf::Vector3f pos);

	void update(sf::Time deltaTime);

	void queueCommand(BasicCommand* command);

	void render(sf::RenderWindow* win);

	// Setters

	void setStatus(EnemyState state);

	void clearCommandsQueue();

	// Getters

	CommandType getCommandID();

	bool commandQueueEmpty();

	EnemyState getStatus();

	float getViewRange();

};