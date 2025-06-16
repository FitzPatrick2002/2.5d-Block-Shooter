import MapsMenuList;
import MenuTile;
import <iostream>;
import GameManager;

import linAlg;

import <filesystem>;
import <regex>;
namespace fs = std::filesystem;

// Comments DONE

//Constructor
// 1. Preapre the state - load font and set window.
// 2. Read the maps from Game/Maps direcotry.
// 3. Setup the UI.
MapsMenuList::MapsMenuList(sf::RenderWindow* win = nullptr) : GameState(win){
	this->loadFont("Game\\Resources\\Fonts\\Play-Bold");
	this->readMapsFromDirectory();
	this->prepareMenuOptions();
	this->menuView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
}

MapsMenuList::~MapsMenuList() {

}

// Read the maps names from Game/Maps direcotry and save them into maps_names vector.
// 1. Check if Game/Maps/ exists.
// 2. Iterate over files, if they are regular files, read their names, check extension.
// 3. If it is .txt, then remove it and save the name to the map names vector.

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

// This would be used in case we wanted a scalable window. 
// Doesn't work properly.
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

// Loads the font.
void MapsMenuList::loadFont(std::string file_name) {

	if (this->tiles_font.loadFromFile(file_name + ".ttf")) {
		
	}
	else {
		std::cerr << "Cannot load font\n";
	}
}

// Set up the tiles in UI.
// 1. Read the window settings (width and height) in order to relatively position the tiles in the menu.
// 2. Resize the options vector
// 3. Iterate over the tiles and configure them (set position, text, etc.).

void MapsMenuList::prepareMenuOptions() {
	this->options.resize(5);

	float rectWidth = 300.0f;
	float rectHeight = 40.0f;

	float spaceBetween = rectHeight * 0.8f;
	float topCoord = (float(this->window->getSize().y) - (spaceBetween * 4.0f + rectHeight * 5.0f)) / 2.0f;

	this->options.clear();
	this->options.resize(this->maps_names.size());

	for (int i = 0; auto & e : this->options) {
		e.setSize(sf::Vector2f(rectWidth, rectHeight));
		e.setPos(sf::Vector2f(100, topCoord + i * (rectHeight + spaceBetween)));
		e.setOutlineThickness(1.0f);
		e.setOutlineColor(sf::Color(0, 255, 0, 255));
		e.setFillColor(sf::Color::Black);
		e.initText(this->tiles_font, maps_names[i]);
		i++;
	}
}


// Read the user input from the keyboard and mouse.
// If mouse left mouse button was pressed launch handleMenuInput();
void MapsMenuList::handleUserInput() {

	while (this->window->pollEvent(this->stateEvent)) {

		switch (this->stateEvent.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:

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

// If any tile is hovered by the mouse during a click, map is chosen and player is redirected to main menu.
void MapsMenuList::handleMenuInput() {
	sf::Vector2f mouse_pos = this->getMousePosition();

	for (int i = 0; i < this->options.size(); i++) { // Iterate over the tiles
		if (this->options[i].containsMouse(mouse_pos)) {
			CombatMapData::getCombatMapData().setMapToLoad(this->options[i].getText().getString()); // Set the map which is to be loaded
			GameManager::getManager().createNewState(GameStateEnum::MainMenu, true); // Go back to main menu
			return;
		}
	}
}

// Returns the mouse position.
sf::Vector2f MapsMenuList::getMousePosition() {
	sf::Vector2i pos = sf::Mouse::getPosition(*this->window);
	sf::Vector2f mouse_pos = this->window->mapPixelToCoords(pos);

	return mouse_pos;
}

void MapsMenuList::update(sf::Time deltaTime) {
	this->render();
	this->handleUserInput();
}

// Renders the menu in the window
void MapsMenuList::render() {
	this->window->clear();

	// Draw the tiles and text on them.
	for (auto& e : this->options) {
		this->window->draw(e);
		this->window->draw(e.getText());
	}

	this->window->display();
}