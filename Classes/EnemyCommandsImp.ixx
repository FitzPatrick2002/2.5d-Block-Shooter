import EnemyCommands;

import <iostream>;
import <functional>;
import <list>;
import <mutex>;
import <thread>;

import Enemy;

import <SFML/Graphics.hpp>;
import linAlg;

// Here forwar declaaration of Enemy


BasicCommand::BasicCommand(CommandType t) {
	type = t;
}
BasicCommand::~BasicCommand() {

}

bool BasicCommand::isFinished() {
	return this->finished;
}

CommandType BasicCommand::getType() {
	return this->type;
}

Follow_Path::Follow_Path(std::vector<sf::Vector2i>& pp, Enemy* e) :BasicCommand(CommandType::FollowPath) {
	this->path = pp;
	this->enemy_handle = e;
	
}

Follow_Path::~Follow_Path() {
	this->enemy_handle = nullptr; // Is it even necessary? 
}

// 1. Set Specific destination
// 2. Travel and check if arrived at the point
// 3. Stop

// I guess we'll just queue a set of Start_movement_Che_if_at_destination and Stop commands
void Follow_Path::operator()() {
	sf::Vector2f new_vel = makeVersor(this->enemy_handle->getWorld_XY(), v2i_to_v2f(path[0]));
	//float vel_mag = 2.0f;
	float vel_mag = CombatMapData::getCombatMapData().getEnemySpeed();

	Start_movement start(new_vel, vel_mag);
	Check_if_at_destination travel(v2i_to_v2f(path[0]));
	Stop_movement stop;

}

Start_movement::Start_movement(sf::Vector2f newVelVers, float newVel) : BasicCommand(CommandType::StartMovement) {
	this->new_vel_versor = newVelVers;
	this->new_velocity = newVel;

}

sf::Vector2f Start_movement::getNewVelocityVersor() {
	return this->new_vel_versor;
}

float Start_movement::getNewVelocityMagnitude() {
	return this->new_velocity;
}

void Start_movement::operator()(sf::Vector3f& vel_versor) {
	vel_versor.x = new_vel_versor.x;
	vel_versor.y = new_vel_versor.y;

	finished = true;
}

void Start_movement::operator()(float& velocity) {
	velocity = new_velocity;

	finished = true;
}

void Start_movement::operator()(sf::Vector3f& vel_versor, float& velocity) {
	vel_versor.x = new_vel_versor.x;
	vel_versor.y = new_vel_versor.y;
	velocity = new_velocity;

	finished = true;
}

Stop_movement::Stop_movement() : BasicCommand(CommandType::StopMovement) {

}

void Stop_movement::operator()(sf::Vector3f& vel_versor) {
	vel_versor = sf::Vector3f(0, 0, 0);
	finished = true;
}

void Stop_movement::operator()(float& velocity) {
	velocity = 0.0f;
	finished = true;
}

void Stop_movement::operator()(sf::Vector3f& vel_versor, float& velocity) {
	vel_versor = sf::Vector3f(0.0f, 0.0f, 0.0f);
	velocity = 0.0f;
	finished = true;
}

Check_if_at_destination::Check_if_at_destination(sf::Vector2f dest) : BasicCommand(CommandType::CheckIfAtDestination) {

	this->destination = dest;
}

Check_if_at_destination::~Check_if_at_destination() {

}

// Return true if in the next movement step we would be farther away from the destination than we are now
void Check_if_at_destination::operator()(sf::Vector2f current_position, sf::Vector2f vel_versor, float velocity) {

	sf::Vector2f next_position = velocity * 0.01f * vel_versor + current_position;
	float current_dist = vectorLength(current_position - destination);
	float next_dist = vectorLength(next_position - destination);

	//std::cout << "Current dist: " << current_dist << " next dist: " << next_dist << "\n";
	//std::cout << "Verdict: " << (next_dist > current_dist) << "\n";;

	finished = (next_dist > current_dist);
}

sf::Vector2f Check_if_at_destination::getDestination() {
	return this->destination;
}


Wait::Wait(float rfsh_t) : BasicCommand(CommandType::Wait) {
	total_time = 0.0f;
	refresh_time = rfsh_t;

}
Wait::~Wait() {

}

void Wait::operator()() {
	total_time += clock.restart().asSeconds();

	if (total_time > refresh_time)
		this->finished = true;
}

float Wait::getRefreshTime() {
	return this->refresh_time;
}

// class Shoot

Shoot::Shoot(std::list<Bullet>* b, sf::Vector2f player_pos) : BasicCommand(CommandType::Shoot){
	this->player_position = player_pos;
	this->bullets_list_handle = b;
}

Shoot::~Shoot() {

}

std::mutex bullets_mutex_temp;

void Shoot::operator()(sf::Vector2f enemy_pos) {
	Bullet b;
	std::cout << this->player_position - enemy_pos << "\n";

	sf::Vector2f versor = makeVersor(this->player_position - enemy_pos);
	versor = 2.0f * versor;

	b.init(enemy_pos + versor, this->player_position);
	
	{
		std::lock_guard<std::mutex> mt(bullets_mutex_temp);
		
		this->bullets_list_handle->push_back(b);
	}
	this->finished = true;
}

std::list<Bullet>* Shoot::getBulletsList() {
	return this->bullets_list_handle;
}

sf::Vector2f Shoot::getPlayerPos() {
	return this->player_position;
}
