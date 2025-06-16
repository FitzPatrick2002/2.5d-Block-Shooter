import Enemy;
import Player;

import linAlg;

import <iostream>;

Enemy::Enemy() {
	
	this->current_command = nullptr;
	this->current_status = EnemyState::Patrol;
	//this->view_range = 7.0f;
	this->view_range = CombatMapData::getCombatMapData().getEnemyDetectionRange();
}

Enemy::~Enemy() {

}

void Enemy::init(sf::Vector3f pos) {

	this->worldPos = pos;

	this->velMag = 0.0f;
	this->velVersor = sf::Vector3f(0.0f, 0.0f, 0.0f);

	sf::Vector3f dims(1.0f, 1.0f, 1.0f);
	this->model_3d.init(pos, dims);
}

void Enemy::performCommand() {

	//std::cout << "Current queue: " << commands_queue.size() << "\n";

	if (this->current_command != nullptr) {

		if (current_command->isFinished()) {
			// std::cout << "command finished" << static_cast<int>(current_command->getType()) << "\n";
			//std::cout << "queuel length: " << commands_queue.size() << "\n";
			commands_queue.pop();
			if (not commands_queue.empty()) {

				this->current_command = commands_queue.front();
			}
			else {
				this->current_command = nullptr;
				return;
			}
		}

		//std::cout << "Performing command: " << current_command << "\n";
		switch (current_command->getType()) {
		case CommandType::StartMovement:

			(*(std::dynamic_pointer_cast<Start_movement>(current_command)))(this->velVersor, this->velMag);
			break;
		case CommandType::CheckIfAtDestination:

			(*(std::dynamic_pointer_cast<Check_if_at_destination>(current_command)))(this->getWorld_XY(), this->getVelVersor_XY(), this->velMag);
			break;
		case CommandType::StopMovement:

			(*(std::dynamic_pointer_cast<Stop_movement>(current_command)))(this->velVersor, this->velMag);
			break;
		case CommandType::Wait:

			(*(std::dynamic_pointer_cast<Wait>(current_command)))();
			break;
		case CommandType::Shoot:

			(*(std::dynamic_pointer_cast<Shoot>(current_command)))(this->getWorld_XY());
			break;
		default:
			std::cout << "Unknown command for an Enemy class, code: " << static_cast<int>(current_command->getType()) << "\n";
			break;
		}

	}
}

void Enemy::queueCommand(BasicCommand* command) {

	if (command == nullptr) {
		std::cerr << "You tried to queue an empty command\n";
		return;
	}
	// We need copy constructors for each command I guess?
	switch (command->getType()) {
	case CommandType::Wait:

		this->commands_queue.push(std::make_shared<Wait>(dynamic_cast<Wait*>(command)->getRefreshTime()));
		this->current_command = this->commands_queue.front();
		break;
	case CommandType::StartMovement:

		this->commands_queue.push(std::make_shared<Start_movement>(dynamic_cast<Start_movement*>(command)->getNewVelocityVersor(), dynamic_cast<Start_movement*>(command)->getNewVelocityMagnitude()));
		this->current_command = this->commands_queue.front();

		break;
	case CommandType::CheckIfAtDestination:

		this->commands_queue.push(std::make_shared<Check_if_at_destination>(dynamic_cast<Check_if_at_destination*>(command)->getDestination()));
		this->current_command = this->commands_queue.front();

		break;
	case CommandType::StopMovement:

		this->commands_queue.push(std::make_shared<Stop_movement>());
		this->current_command = this->commands_queue.front();

		break;
	case CommandType::Shoot:

		this->commands_queue.push(std::make_shared<Shoot>(dynamic_cast<Shoot*>(command)->getBulletsList(), dynamic_cast<Shoot*>(command)->getPlayerPos()));
		this->current_command = this->commands_queue.front();

		break;
	default:
		std::cerr << "An unknown type of command has been queued and so it wasn't queued for the enemy list of commands\n";
		break;
	}
}

void Enemy::updatePosition(sf::Time deltaTime) {
	sf::Vector3f next_position = this->worldPos + this->velMag * deltaTime.asSeconds() * this->velVersor;

	std::cout << "Vel mag = " << this->velMag << "\n";
	std::cout << "Vel mag = " << this->velMag << "\n";
	std::cout << "Vel mag = " << this->velMag << "\n";
	std::cout << "Vel mag = " << this->velMag << "\n";

	this->setWorldPos(next_position);
	this->model_3d.setWorldPosition(next_position);
}

void Enemy::update(sf::Time deltaTime) {

	//std::cout << "Commadns of " << this << " amount: " << this->commands_queue.size() << "\n";

	this->performCommand();

	this->updatePosition(deltaTime);
}

void Enemy::render(sf::RenderWindow* win) {

	this->model_3d.render(win);
}

// Setters

void Enemy::setStatus(EnemyState state) {

	this->current_status = state;
}

void Enemy::clearCommandsQueue() {
	this->commands_queue = std::queue<std::shared_ptr<BasicCommand>>();
	this->current_command = nullptr;

	Stop_movement stop;
	this->queueCommand(&stop);
}

// Getters

CommandType Enemy::getCommandID() {
	if (current_command == nullptr)
		return CommandType::Undefined;

	return (current_command->getType());
}

bool Enemy::commandQueueEmpty() {
	
	return this->commands_queue.empty();
}

EnemyState Enemy::getStatus() {
	
	return this->current_status;
}

float Enemy::getViewRange() {
	
	return this->view_range;
}