export module EnemyCommands;

import <iostream>;
import <functional>;
import <memory>;
import <list>;

import <SFML/Graphics.hpp>;
import linAlg;
import Bullet;

export class Enemy;

export enum class CommandType
{
	StartMovement,
	StopMovement,
	CheckIfAtDestination,
	Wait,
	FollowPath,
	Shoot,
	Undefined
};

export class BasicCommand {
protected:
	bool finished = false;
	CommandType type;

public:

	BasicCommand(CommandType t);
	virtual ~BasicCommand();

	template<typename... Args>
	void operator()(Args... args) {

	}

	bool isFinished();

	CommandType getType();

};

export struct Follow_Path : BasicCommand{
private:
	std::vector<sf::Vector2i> path;
	Enemy* enemy_handle;

public:

	Follow_Path(std::vector<sf::Vector2i>& pp, Enemy* e);
	~Follow_Path();

	void operator()();

};

export class Start_movement : public BasicCommand {
private:

	sf::Vector2f new_vel_versor;
	float new_velocity;

public:

	Start_movement(sf::Vector2f newVelVers, float newVel);

	~Start_movement() = default;

	sf::Vector2f getNewVelocityVersor();

	float getNewVelocityMagnitude();

	void operator()(sf::Vector3f& vel_versor);

	void operator()(float& velocity);

	void operator()(sf::Vector3f& vel_versor, float& velocity);

};

export class Stop_movement : public BasicCommand {
private:

public:

	Stop_movement();

	~Stop_movement() = default;

	void operator()(sf::Vector3f& vel_versor);

	void operator()(float& velocity);

	void operator()(sf::Vector3f& vel_versor, float& velocity);
};

export class Check_if_at_destination : public BasicCommand {
private:
	sf::Vector2f destination;

public:

	Check_if_at_destination(sf::Vector2f dest);

	~Check_if_at_destination();

	// Return true if in the next movement step we would be farther away from the destination than we are now
	void operator()(sf::Vector2f current_position, sf::Vector2f vel_versor, float velocity);

	sf::Vector2f getDestination();

};

export class Wait : public BasicCommand {
private:
	float total_time;
	float refresh_time;

	sf::Clock clock;
public:

	Wait(float rfsh_t);
	~Wait();

	void operator()();

	float getRefreshTime();
};

// Combat commands

export class Shoot : public BasicCommand {
private:
	std::list<Bullet>* bullets_list_handle;
	sf::Vector2f player_position;

public:

	Shoot(std::list<Bullet>* b, sf::Vector2f player_pos);
	~Shoot();

	void operator()(sf::Vector2f enemy_pos);

	std::list<Bullet>* getBulletsList();

	sf::Vector2f getPlayerPos();

};