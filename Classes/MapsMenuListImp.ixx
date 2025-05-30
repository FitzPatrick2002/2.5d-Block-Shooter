import MapsMenuList;
import MenuTile;
import <iostream>;
import GameManager;

import linAlg;

import <filesystem>;
import <regex>;
namespace fs = std::filesystem;


MapsMenuList::MapsMenuList(sf::RenderWindow* win = nullptr) : GameState(win){

	this->loadFont("Game\\Resources\\Fonts\\Play-Bold");
	this->readMapsFromDirectory();

	this->prepareMenuOptions();

	this->menuView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

}

MapsMenuList::~MapsMenuList() {

}



// 1. Check if Game/Maps/ exists
// 2. Iterate over files, if they are regular files, read their names, czeck extension.
// 3. If it is txt, then remove it and save the name to the map names vector

void MapsMenuList::readMapsFromDirectory() {
	fs::path path_to_maps("Game\\Maps");

	if (fs::exists(path_to_maps)) {
		auto itr = fs::directory_iterator(path_to_maps);

		for (fs::directory_entry e : itr) {
			std::string name = e.path().filename().string();
			std::string extension = e.path().extension().string();
			
			name.erase(name.find(extension), extension.size());
			this->maps_names.push_back(name);
		}

	}
	else {
		std::cerr << "Folder Game\\Maps is missing, can't read maps, load fonts, nothing\n";
	}
}

void MapsMenuList::resizeView() {
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

void MapsMenuList::loadFont(std::string file_name) {

	if (this->tiles_font.loadFromFile(file_name + ".ttf")) {
		
	}
	else {
		std::cerr << "Cannot load font\n";
	}
}

void MapsMenuList::prepareMenuOptions() {
	this->options.resize(5);

	float rectWidth = 300.0f;
	float rectHeight = 40.0f;

	float spaceBetween = rectHeight * 0.8f;
	float topCoord = (float(this->window->getSize().y) - (spaceBetween * 4.0f + rectHeight * 5.0f)) / 2.0f;

	// This part will be differnt -> different options -> map namwes, alsp different events on clicking
	this->options.clear();
	this->options.resize(this->maps_names.size());

	for (int i = 0; auto & e : this->options) {

		e.setSize(sf::Vector2f(rectWidth, rectHeight));
		e.setPos(sf::Vector2f(100, topCoord + i * (rectHeight + spaceBetween)));
		std::cout << topCoord + i * spaceBetween << "\n";

		e.setOutlineThickness(1.0f);
		e.setOutlineColor(sf::Color(0, 255, 0, 255));

		e.setFillColor(sf::Color::Black);

		e.initText(this->tiles_font, maps_names[i]);

		i++;
	}

	// Add additional tile for coming back to the main menu



}

// TO DO: sf::view so that the rectangles do not resize.
// In the background there will be a blurred printscreen 
// (moving and then fluently changing to a different one)
// of the gameplay
void MapsMenuList::handleUserInput() {

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

void MapsMenuList::handleMenuInput() {
	sf::Vector2f mouse_pos = this->getMousePosition();

	for (int i = 0; i < this->options.size(); i++) {
		if (this->options[i].containsMouse(mouse_pos)) {
			CombatMapData::getCombatMapData().setMapToLoad(this->options[i].getText().getString()); // Set the map which is to be loaded

			GameManager::getManager().createNewState(GameStateEnum::MainMenu, true);
			return;
		}

	}

	if (this->options[0].containsMouse(mouse_pos)) {
		//GameManager::getManager().createNewState(GameStateEnum::Combat, true);


		return;
	}
	else if (this->options[1].containsMouse(mouse_pos)) {

	}
	else if (this->options[2].containsMouse(mouse_pos)) {

	}
	else if (this->options[3].containsMouse(mouse_pos)) {

	}
	else if (this->options[4].containsMouse(mouse_pos)) {

	}

}

sf::Vector2f MapsMenuList::getMousePosition() {
	sf::Vector2i pos = sf::Mouse::getPosition(*this->window);
	sf::Vector2f mouse_pos = this->window->mapPixelToCoords(pos);

	return mouse_pos;
}

void MapsMenuList::update(sf::Time deltaTime) {
	//this->window->setView(this->menuView);

	this->render();

	this->handleUserInput();
}

//void update(); // From polymorphism
void MapsMenuList::render() {

	this->window->clear();

	for (auto& e : this->options) {
		this->window->draw(e);
		this->window->draw(e.getText());
	}
	this->window->display();
}