export module EnemyCommands;

import <iostream>;
import <functional>;

import <SFML/Graphics.hpp>;
import linAlg;


export enum class CommandType
{
	StartMovement,
	StopMovement,
	CheckIfAtDestination,
	Wait,
	Undefined
};

export class BasicCommand {
protected:
	bool finished = false;
	CommandType type;

public:

	BasicCommand(CommandType t) {
		type = t;
	}
	virtual ~BasicCommand() {

	}

	template<typename... Args>
	void operator()(Args... args) {

	}

	bool isFinished() {
		return this->finished;
	}

	CommandType getType() {
		return this->type;
	}

};

export class Start_movement : public BasicCommand {
private:

	sf::Vector2f new_vel_versor;
	float new_velocity;

public:

	Start_movement(sf::Vector2f newVelVers, float newVel) : BasicCommand(CommandType::StartMovement) {
		this->new_vel_versor = newVelVers;
		this->new_velocity = newVel;

	}

	~Start_movement() = default;

	sf::Vector2f getNewVelocityVersor() {
		return this->new_vel_versor;
	}

	float getNewVelocityMagnitude() {
		return this->new_velocity;
	}

	void operator()(sf::Vector3f& vel_versor) {
		vel_versor.x = new_vel_versor.x;
		vel_versor.y = new_vel_versor.y;

		finished = true;
	}

	void operator()(float& velocity) {
		velocity = new_velocity;

		finished = true;
	}

	void operator()(sf::Vector3f& vel_versor, float& velocity) {
		vel_versor.x = new_vel_versor.x;
		vel_versor.y = new_vel_versor.y;
		velocity = new_velocity;

		finished = true;
	}

};

export class Stop_movement : public BasicCommand {
private:

public:

	Stop_movement() : BasicCommand(CommandType::StopMovement) {

	}

	~Stop_movement() = default;

	void operator()(sf::Vector3f& vel_versor) {
		vel_versor = sf::Vector3f(0, 0, 0);
		finished = true;
	}

	void operator()(float& velocity) {
		velocity = 0.0f;
		finished = true;
	}

	void operator()(sf::Vector3f& vel_versor, float& velocity) {
		vel_versor = sf::Vector3f(0.0f, 0.0f, 0.0f);
		velocity = 0.0f;
		finished = true;
	}
};

export class Check_if_at_destination : public BasicCommand {
private:
	sf::Vector2f destination;

public:

	Check_if_at_destination(sf::Vector2f dest) : BasicCommand(CommandType::CheckIfAtDestination) {

		this->destination = dest;
	}

	~Check_if_at_destination() {

	}

	// Return true if in the next movement step we would be farther away from the destination than we are now
	void operator()(sf::Vector2f current_position, sf::Vector2f vel_versor, float velocity) {

		sf::Vector2f next_position = velocity * 0.01f * vel_versor + current_position;
		float current_dist = vectorLength(current_position - destination);
		float next_dist = vectorLength(next_position - destination);

		//std::cout << "Current dist: " << current_dist << " next dist: " << next_dist << "\n";
		//std::cout << "Verdict: " << (next_dist > current_dist) << "\n";;

		finished = (next_dist > current_dist);
	}

	sf::Vector2f getDestination() {
		return this->destination;
	}

};

export class Wait : public BasicCommand {
private:
	float total_time;
	float refresh_time;

	sf::Clock clock;
public:

	Wait(float rfsh_t) : BasicCommand(CommandType::Wait) {
		total_time = 0.0f;
		refresh_time = rfsh_t;

	}
	~Wait() {

	}

	void operator()() {
		total_time += clock.restart().asSeconds();

		if (total_time > refresh_time)
			this->finished = true;
	}

	float getRefreshTime() {
		return this->refresh_time;
	}
};