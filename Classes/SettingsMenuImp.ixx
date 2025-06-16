import SettingsMenu;
import GameManager;
import linAlg;

import <fstream>;
import <regex>;
import <iostream>;
import <filesystem>;

namespace fs = std::filesystem;

// Comments done

// Constructor
// 1. Prepare the window like in other states
// 2. Download settings from files
// 3. Set up tiles in the settings_2 map 
SettingsMenu::SettingsMenu(sf::RenderWindow* window) : GameState(window){
	this->menuView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	//std::cout << "Current path: " << fs::current_path() << "\n";

	this->loadFont("Game\\Resources\\Fonts\\Play-Bold");

	this->readSettingsFiles();
	this->setupArrows();

	// If settings files were correctly read, set up the current entity.
	// If not, set the current_entity to no_entity.
	if (this->settings_2.size() > 0) {
		this->current_entity = this->settings_2.begin()->first;
		if (this->accessTilesVectorFromObject(this->current_entity).size() > 0)
			this->current_entity_variable = this->accessTilesVectorFromObject(this->current_entity).begin()->first;
		else
			this->current_entity_variable = "no_variable";
	}
	else {
		this->current_entity = "no_entity";
	}
}

SettingsMenu::~SettingsMenu() {

}

// Private settings:

// Creates an empty vector in the settings_2 map.
// object - name of the object that we want to create in the map.
void SettingsMenu::addObjectToSettings_2(std::string object) {
	this->settings_2[object];
}

// Function that adds a new tile to the vector of settings. 
// object - name of the object to which we want to add a setting.
// variable - name of the variable of the object, to which the setting is referring to.
// tile - copy of a set up tile, in which we want to display the name of the setting. It will be added to the vector of settings for the given object
void SettingsMenu::addTileToObjectSettings_2(std::string object, std::string variable, MenuTile tile) {
	auto itr = this->settings_2.find(object); // Check if given object exists, if so, proceed, if not, print message

	if (itr != this->settings_2.end()) {
		auto itr_inner = std::find_if(settings_2[object].begin(), settings_2[object].end(), [variable](std::pair<std::string, MenuTile> elem) -> bool {
			return elem.first == variable;
			}); // Check if given variable exists if so, add the tile with a new variable, if not, print message.
		if (itr_inner == itr->second.end()) {
			itr->second.push_back(std::pair<std::string, MenuTile>(variable, tile));
		}
		else {
			std::cerr << "Variable: " << variable << " already exists for object: " << object << "\n";
		}
	}
	else {
		std::cerr << "Cannot add a tile to variable: " << variable << " to object: " << object << ". Object doesn't exist\n";
	}
}

// This function returns the tile of the given variable of specified object from the settings_2 map.

// object - object whose settings we want to read.
// variable - name of the setting whose tile we want to get.
MenuTile SettingsMenu::getSettings_2Tile(std::string object, std::string variable) {
	// If object of given type exists in the map, check if it has variable with specified name. If so, return the tile
	if (this->settings_2.find(object) != this->settings_2.end()) {
		auto itr = std::find_if(settings_2[object].begin(), settings_2[object].end(), [variable](std::pair<std::string, MenuTile> elem) -> bool {
			return elem.first == variable;
			});
		if (itr != settings_2[object].end()) {
			return itr->second;
		}
		else {
			std::cerr << "Object of type: " << object << " has no variable: " << variable << "\n";
		}
	}
	else {
		std::cerr << "No such object in settings: " << object << "\n";
	}

	return MenuTile();
}

// Function returns the vector of tiles of a specified object.
// If object is not found, it returns an empty vector.

// object - name of the object whose vector of tiles we want to get.
std::vector<std::pair<std::string, MenuTile>> SettingsMenu::accessTilesVectorFromObject(std::string object) {
	auto itr = this->settings_2.find(object);
	if (itr != this->settings_2.end()) {
		return itr->second;
	}

	std::vector<std::pair<std::string, MenuTile>> dummy;
	return dummy;
}

//This function is used to change text inside a tile of a specified variable of a specified object.

// object - object whose tile we want to alter.
// variable - name of a setting which we want to change.
// new_text - text with which we want to replace the old variable name.
void SettingsMenu::alterTileTextSettings_2(std::string object, std::string variable, std::string new_text) {
	if (this->settings_2.find(object) != this->settings_2.end()) {
		auto itr = std::find_if(settings_2[object].begin(), settings_2[object].end(), [variable](std::pair<std::string, MenuTile> elem) -> bool {
			return elem.first == variable;
			});

		if (itr != settings_2[object].end()) {
			itr->second.setText(new_text);
		}
		else {
			std::cerr << "Object of type: " << object << " has no variable: " << variable << "\n";
		}
	}
	else {
		std::cerr << "No such object in settings: " << object << "\n";
	}
}

// Writes the current values of attributes into the intermediate object CombatMapData. 
// Later specific objects (player, bullet, etc.) will download these values from it. 
// Values are extracted from the text inside tiles, it follows the format:
// variable_name: float_value.
void SettingsMenu::setCombatMapDataAttributesValues() {
	std::regex number_regex("(\\d+\\.\\d+)"); // regex for extracting the float values.
	std::smatch m;

	for (auto& elem : this->settings_2) { 
		std::string object = elem.first; // Name of the currently saved object.
		for (auto& p : elem.second) {
			std::string variable = p.first; 
			std::string data = p.second.getText().getString(); // Get the text from tile.

			std::regex_search(data, m, number_regex);
			float val = std::atof(m[1].str().c_str()); // Extract the float value of the setting.

			CombatMapData::getCombatMapData().setFloatParameter(object, variable, val); // Save it to the CombatMapData.
		}
	}

	//CombatMapData::getCombatMapData().printFloatContents();
}

// Public settings

// Function that handles the user keyboard and mouse inputs
// m - save the settings and go back to main menu
// mouse left click - interaction with tiles
void SettingsMenu::handleUserInput() {

	while (this->window->pollEvent(this->stateEvent)) {
		switch (this->stateEvent.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:

			if (this->stateEvent.key.code == sf::Keyboard::M) {
				this->setCombatMapDataAttributesValues(); // Save the setting values to the CombatMapData. Program will access them from here. 
				GameManager::getManager().createNewState(GameStateEnum::MainMenu, true); // Pop the current state from stack and create main menu.
				return;
			}

			break;
		case sf::Event::MouseButtonPressed:
			this->handleMenuInput(); // Check what the click means. 
			break;
		default:
			break;
		}
	}

}

// Setup of the arrows.
void SettingsMenu::setupArrows() {

	MenuTile dummy;

	// Position the arrows based on the window size.
	sf::Vector2f window_size = sf::Vector2f(this->window->getSize().x, this->window->getSize().y);
	sf::Vector2f tile_size(this->window->getSize().x / 8.0f, 30.0f);
	sf::Vector2f initial_tile_pos(window_size.x * 1.0f / 8.0f, window_size.y * 1.0f / 5.0f);
	sf::Vector2f delta_pos = 1.4f * tile_size;
	delta_pos.x = 0.0f;

	dummy.setPosition(initial_tile_pos);
	dummy.setSize(tile_size);
	dummy.setOutlineThickness(1.0f);
	dummy.setOutlineColor(sf::Color(0, 255, 0, 255));
	dummy.setFillColor(sf::Color::Black);
	dummy.initText(this->file_font, "increase");

	this->arrow_up = dummy;

	dummy.setPosition(initial_tile_pos + delta_pos);
	dummy.initText(this->file_font, "decrease");

	this->arrow_down = dummy;

}

// Function reads the settings from .txt files and fills the settings_2 map.

// 1. Iterate over files in the settings folder.
// 2. Count them and prepare the tiles mappings and names.
// 3. Create a new map element -> accessible by the name of object that it is referring to.
// 3. Read each file, assign each tile text to display.
void SettingsMenu::readSettingsFiles() {
	fs::path settings_path("Game\\Resources\\Settings");

	// 0. Get the size of the window for relative tiles positioning.
	sf::Vector2f window_size = sf::Vector2f(this->window->getSize().x, this->window->getSize().y);
	sf::Vector2f tile_size(this->window->getSize().x / 4.0f, 30.0f);
	sf::Vector2f initial_tile_pos(window_size.x * 3.0f / 8.0f, window_size.y * 1.0f / 5.0f);
	sf::Vector2f delta_pos = 1.4f * tile_size;
	delta_pos.x = 0.0f;

	// If directory with settings exists attempt to read the files.
	if (fs::exists(settings_path)) {
		auto itr = fs::directory_iterator(settings_path);

		// 1. Iterate over files in the settings folder.
		for (fs::directory_entry e : itr) {
			std::string file_name = e.path().filename().string();
			std::string extension = e.path().extension().string();

			// Read the file
			std::ifstream file(settings_path.string() + "\\" + file_name);

			// Preapre place for data from file in the map.
			file_name.erase(file_name.find(extension), extension.size()); // Delete the .txt from the file name, leave only the object name.

			if (file.is_open()) {				
				this->addObjectToSettings_2(file_name); // Create an entry in the map for the new object.
				CombatMapData::getCombatMapData().addFloatObject(file_name); // Create a new object for the map of objects in the MapComabtData.

				// Set up the top tile.
				// Add the initial tile, name of the object.
				MenuTile tile_0;
				tile_0.setPosition(initial_tile_pos);
				tile_0.setSize(tile_size);
				tile_0.setOutlineThickness(1.0f);
				tile_0.setOutlineColor(sf::Color(0, 255, 0, 255));
				tile_0.setFillColor(sf::Color::Black);
				tile_0.initText(this->file_font, file_name);

				this->addTileToObjectSettings_2(file_name, file_name, tile_0); // Add the top tile.

				std::string line;

				std::regex data_regex("([\\w]+):?\\s*(-*\\d+\\.*\\d+)"); // Regex for extracting the name of the variable and the value of it. 
				std::smatch m;

				// Iterate over all lines of the file. 
				float line_num = 2.0f;
				while (std::getline(file, line)) {
					// If line matched, check to which variable it is referring. 
					if (std::regex_match(line, m, data_regex)) {
						std::string tile_text = m[1].str();
						float value = std::atof(m[2].str().c_str());
						
						// Set the value in the CombatMapData (Math.ixx)
						CombatMapData::getCombatMapData().addFieldForFloatObject(file_name, tile_text);
						CombatMapData::getCombatMapData().setFloatParameter(file_name, tile_text, value);

						MenuTile tile; // Setup the new tile.
						tile.setPosition(initial_tile_pos + delta_pos * line_num);
						tile.setSize(tile_size);
						tile.setOutlineThickness(1.0f);
						tile.setOutlineColor(sf::Color(0, 255, 0, 255));
						tile.setFillColor(sf::Color::Black);
						tile.initText(this->file_font, line);
						
						this->addTileToObjectSettings_2(file_name, tile_text, tile); // Add the new tile to the vector of tiles of currently serviced object. 
					}

					line_num += 1.0f;
				}
			}
			else {
				std::cerr << "Could not read data from settings file: " << file_name << "\n";
			}
		}

	}
	else {
		std::cerr << "Folder Game\\Resources\\Settings is missing, can't read settings, load fonts, nothing\n";
	}

	//CombatMapData::getCombatMapData().printFloatContents();
}

// Loads the font.
void SettingsMenu::loadFont(std::string file_name) {
	if (this->file_font.loadFromFile(file_name + ".ttf")) {
		std::clog << "Loaded font: " << file_name << "\n";
	}
	else {
		std::cerr << "Cannot load font\n";
	}
}

// Update the current frame. 
// Called by the GameManager in the main loop.
void SettingsMenu::update(sf::Time deltaTime) {

	this->render();
	this->handleUserInput();
}

void SettingsMenu::setInitialCurrentVariable() {

}

// Ths is what happens when the mouse is clicked
void SettingsMenu::handleMenuInput() {
	CombatMapData::getCombatMapData().printFloatContents();
	sf::Vector2f mouse_pos = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));

	// Check if any tile contains the mouse
	// If during click, mouse hovers over one of the settings, set the current_entity_variable to the name of this setting.

	if (this->accessTilesVectorFromObject(this->current_entity).size() > 0) {
		auto vec = this->accessTilesVectorFromObject(this->current_entity);
		for (auto& e : vec) {
			if (e.second.getGlobalBounds().contains(mouse_pos)) {
				this->current_entity_variable = e.first;
			}
		}
	}

	// If we click on the top tile (the one storing the object name), the settings that we are modifying are selected for that new object. 
	if (this->accessTilesVectorFromObject(this->current_entity).begin()->second.getGlobalBounds().contains(mouse_pos)) {
		this->loadSettingsForNextEntity();
	}

	// Handles change of values of settings of objects like map, enemy, player, etc.
	auto change_values = [this](bool increase) {
		// Increase value of the currently selected paramaeter.
		std::string current_string = this->getSettings_2Tile(this->current_entity, this->current_entity_variable).getText().getString();
		float current_value = CombatMapData::getCombatMapData().getFloatParameter(this->current_entity, this->current_entity_variable);

		increase ? current_value += 0.5f : current_value -= 0.5f;

		std::regex float_regex("\\d+\\.\\d*"); // Regex for extracting the float value from the text.
		current_string = std::regex_replace(current_string, float_regex, std::to_string(current_value));
		this->alterTileTextSettings_2(this->current_entity, this->current_entity_variable, current_string);
		CombatMapData::getCombatMapData().setFloatParameter(this->current_entity, this->current_entity_variable, current_value);
		};
	
	//If mouse is pointed at increase or decrese tile, increse / decrese value of the currently selected variable.
	if (this->arrow_up.getGlobalBounds().contains(mouse_pos)) {
		change_values(true);
	}
	else if (this->arrow_down.getGlobalBounds().contains(mouse_pos)) {
		change_values(false);
	}
}

// If currently serviced object has been changed, change the layout.
// Load the name of the currently serviced object to the top tile and load its settings tiles.
void SettingsMenu::loadSettingsForNextEntity() {
	auto itr = this->settings_2.find(this->current_entity);

	if (std::next(itr) == this->settings_2.end()) // If we were modyfying the last object in the map, wrap around back to the first one.
		itr = this->settings_2.begin();
	else
		itr = std::next(itr);

	if (this->settings_2[this->current_entity].size() > 1)
		this->current_entity_variable = std::next(this->settings_2[this->current_entity].begin())->first;
	else
		this->current_entity_variable = "no_variable";

	this->current_entity = itr->first;
}

// Render tiles in the window.
void SettingsMenu::render() {
	this->window->clear();

	// Draw the arrows (increase / decrese).
	this->window->draw(this->arrow_up);
	this->window->draw(this->arrow_up.getText());
	this->window->draw(this->arrow_down);
	this->window->draw(this->arrow_down.getText());

	// Draw the tiles with options
	for (auto& e : this->settings_2[current_entity]) {
		this->window->draw(e.second);
		this->window->draw(e.second.getText());
	}

	this->window->display();
}