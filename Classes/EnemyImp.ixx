import Enemy;

import <iostream>;

Enemy::Enemy() {
	
	Wait wait(1.0f);
	this->queueCommand(&wait);

	this->current_command = commands_queue.front();
	commands_queue.pop();
}

Enemy::~Enemy() {

}

void Enemy::performCommand() {

	if (current_command->isFinished()) {
		// std::cout << "command finished" << static_cast<int>(current_command->getType()) << "\n";

		if (not commands_queue.empty()) {

			this->current_command = commands_queue.front();
			commands_queue.pop();
		}
		else {
			Wait wait(1.0f);

			this->queueCommand(&wait);
			this->current_command = commands_queue.front();
			commands_queue.pop();
		}
	}

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
	default:
		std::cout << "Unknown command for an Enemy class, code: " << static_cast<int>(current_command->getType()) << "\n";
		break;
	}
}

void Enemy::queueCommand(BasicCommand* command) {

	// We need copy constructors for each command I guess?
	switch (command->getType()) {
	case CommandType::Wait:

		this->commands_queue.push(std::make_shared<Wait>(dynamic_cast<Wait*>(command)->getRefreshTime()));
		break;
	case CommandType::StartMovement:

		this->commands_queue.push(std::make_shared<Start_movement>(dynamic_cast<Start_movement*>(command)->getNewVelocityVersor(), dynamic_cast<Start_movement*>(command)->getNewVelocityMagnitude()));
		break;
	case CommandType::CheckIfAtDestination:

		this->commands_queue.push(std::make_shared<Check_if_at_destination>(dynamic_cast<Check_if_at_destination*>(command)->getDestination()));
		break;
	case CommandType::StopMovement:

		this->commands_queue.push(std::make_shared<Stop_movement>());
		break;
	default:
		std::cerr << "An unknown type of command has been queued and so it wasn't queued for the enemy list of commands\n";
		break;
	}
}

void Enemy::updatePosition(sf::Time deltaTime) {
	sf::Vector3f next_position = this->worldPos + this->velMag * deltaTime.asSeconds() * this->velVersor;

	this->setWorldPos(next_position);
}

void Enemy::update(sf::Time deltaTime) {

	this->performCommand();

	this->updatePosition(deltaTime);
}

void Enemy::render(sf::RenderWindow* win) {


}