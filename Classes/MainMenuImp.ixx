import MainMenu;
import GameManager;

import <iostream>;

MainMenu::MainMenu(sf::RenderWindow* w) : GameState(w){

	this->prepareMenuOptions();

	this->testRect.setSize(sf::Vector2f(500.0f, 100.0f));
	this->testRect.setPosition(sf::Vector2f(100.0f, 100.0f));
	this->testRect.setFillColor(sf::Color::Red);

}

MainMenu::~MainMenu() {

}

void MainMenu::prepareMenuOptions() {
	this->options.resize(5);

	float rectWidth = float(this->window->getSize().x) / 3.0f;
	float rectHeight = rectWidth / 7.0f;

	float spaceBetween = rectHeight * 0.8f;
	float topCoord = (float(this->window->getSize().y) - (spaceBetween * 4.0f + rectHeight * 5.0f))/2.0f;

	for (int i = 0;  auto & e : this->options) {
		
		e.setSize(sf::Vector2f(rectWidth, rectHeight));
		e.setPosition(sf::Vector2f(100, topCoord + i*(rectHeight + spaceBetween)));
		std::cout << topCoord + i * spaceBetween << "\n";

		e.setOutlineThickness(5.0f);
		e.setOutlineColor(sf::Color(200, 20, 20, 255));
		
		e.setFillColor(sf::Color::Red);
		i++;
	}

}

// TO DO: sf::view so that the rectangles do not resize.
// In the background there will be a blurred printscreen 
// (moving and then fluently changing to a different one)
// of the gameplay
void MainMenu::handleUserInput() {

	while (this->window->pollEvent(this->stateEvent)) {

		switch (this->stateEvent.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:

			if (this->stateEvent.key.code == sf::Keyboard::M) {
				GameManager::getManager().createNewState(GameStateEnum::Combat, true);
				return;
			}

			break;
		default:
			break;
		}
	}

}

void MainMenu::update(sf::Time deltaTime) {

	this->render();

	this->handleUserInput();
}

//void update(); // From polymorphism
void MainMenu::render() {

	this->window->clear();

	for (auto& e : this->options)
		this->window->draw(e);
	//this->window->draw(this->testRect);

	this->window->display();
}