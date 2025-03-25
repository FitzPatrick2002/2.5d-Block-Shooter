import MainMenu;
import GameManager;

import linAlg;

import <iostream>;

// class MenuTile

// class MainMenu

MainMenu::MainMenu(sf::RenderWindow* w) : GameState(w){
	this->prepareMenuOptions();

	this->menuView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	this->loadFont("Game\\Resources\\Fonts\\Play-Bold");
}

MainMenu::~MainMenu() {

}

void MainMenu::resizeView() {
	sf::Vector2f window_size(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y));
	float window_ratio = window_size.x / window_size.y;

	float original_ratio = 1024.0f / 768.0f;
	sf::Vector2f original_size = sf::Vector2f(1024.0f, 768.0f);

	if (window_ratio > original_ratio)
		original_size.x = original_size.y * window_ratio;
	else
		original_size.y = original_size.x * window_ratio;

	this->menuView.setSize(original_size);
	this->menuView.setCenter(1024.0f / 2.0f, 768.0f / 2.0f);
}

void MainMenu::loadFont(std::string file_name) {

	if (this->tiles_font.loadFromFile(file_name + ".ttf")) {

	}
	else {
		std::cerr << "Cannot load font\n";
	}
}

void MainMenu::prepareMenuOptions() {
	this->options.resize(5);

	float rectWidth = float(this->window->getSize().x) / 3.0f;
	float rectHeight = rectWidth / 7.0f;

	float spaceBetween = rectHeight * 0.8f;
	float topCoord = (float(this->window->getSize().y) - (spaceBetween * 4.0f + rectHeight * 5.0f))/2.0f;

	std::string tiles_texts[5] = {
		"Play",
		"Maps",
		"Options",
		"Credits",
		"Quit"
	};

	for (int i = 0;  auto & e : this->options) {
		
		e.setSize(sf::Vector2f(rectWidth, rectHeight));
		e.setPos(sf::Vector2f(100, topCoord + i*(rectHeight + spaceBetween)));
		std::cout << topCoord + i * spaceBetween << "\n";

		e.setOutlineThickness(1.0f);
		e.setOutlineColor(sf::Color(0, 255, 0, 255));
		
		e.setFillColor(sf::Color::Black);

		e.initText(this->tiles_font, tiles_texts[i]);

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
		case sf::Event::Resized:
			//this->resizeView();

				break;
		case sf::Event::MouseButtonPressed:
			this->handleMenuInput();
			return;

			break;
		default:
			break;
		}
	}

}

void MainMenu::handleMenuInput() {
	sf::Vector2f mouse_pos = this->getMousePosition();

	if (this->options[0].containsMouse(mouse_pos)) {
		GameManager::getManager().createNewState(GameStateEnum::Combat, true);
		return;
	}
	else if (this->options[1].containsMouse(mouse_pos)) {
		GameManager::getManager().createNewState(GameStateEnum::MapChoosing, true);
		return;
	}
	else if (this->options[2].containsMouse(mouse_pos)) {

	}
	else if (this->options[3].containsMouse(mouse_pos)) {

	}
	else if (this->options[4].containsMouse(mouse_pos)) {

	}

}

sf::Vector2f MainMenu::getMousePosition() {
	sf::Vector2i pos = sf::Mouse::getPosition(*this->window);
	sf::Vector2f mouse_pos = this->window->mapPixelToCoords(pos);

	return mouse_pos;
}

void MainMenu::update(sf::Time deltaTime) {
	//this->window->setView(this->menuView);

	this->render();

	this->handleUserInput();
}

//void update(); // From polymorphism
void MainMenu::render() {

	this->window->clear();

	for (auto& e : this->options) {
		this->window->draw(e);
		this->window->draw(e.text);
	}
	this->window->display();
}