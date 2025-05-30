export module SettingsMenu;

import GameState;
import MenuTile;

import <unordered_map>;
import <tuple>;
import <iostream>;
import <regex>;

// We need a system to read the input from the keyboard and display on the tile. 
// Well we could just use the console...
// Maybe later think of something fancy

// File format:
// Settings for different entites are stored in separate files? 
// PlayerSetting.txt
// EnemySettings.txt
// MapSettings.txt
// ...

// How to display them? 
//    <| Player |>
//     | speed  |
//  | fireing rate |

// With <| and |> we can move between different entities settings. 
// Eh this isn't very flexible is it?
// Whatever


export class SettingsMenu : public GameState {
private:
	sf::View menuView;
	//std::unordered_map <std::string, std::vector<std::pair<MenuTile, std::string>>> settings; // The second element is the string that is the key to the modified value, that is stored in Math.ixx -> CombatMapDAta singleton class
	std::unordered_map <std::string, std::unordered_map<std::string, MenuTile>> settings; // The second element is the string that is the key to the modified value, that is stored in Math.ixx -> CombatMapDAta singleton class
	
	std::unordered_map<std::string, std::vector<std::pair<std::string, MenuTile>>> settings_2;

	MenuTile arrow_up;
	MenuTile arrow_down;

	sf::Font file_font;

	std::string current_entity;
	std::string current_entity_variable;

private:

	// create object, create variable for object, get Tile for object, set Tile for object, alter tile text

	void addObjectToSettings_2(std::string object) {
		this->settings_2[object];
	}

	/*void addVariableToSettings_2(std::string object, std::string variable) {
		if (this->settings_2.find(object) != this->settings_2.end()) {
			auto itr = std::find_if(settings_2[object].begin(), settings_2[object].end(), [variable](std::pair<std::string, MenuTile> elem) -> bool {
				return elem.first == variable;
				});

			if (itr == settings_2[object].end()) {
				this->
			}
			else {
				std::cerr << "Object of type: " << object << " has no variable: " << variable << "\n";
			}
		}
		else {
			std::cerr << "No such object in settings: " << object << "\n";
		}
	}*/

	void addTileToObjectSettings_2(std::string object, std::string variable, MenuTile tile);

	MenuTile getSettings_2Tile(std::string object, std::string variable);

	std::vector<std::pair<std::string, MenuTile>> accessTilesVectorFromObject(std::string object);

	void alterTileTextSettings_2(std::string object, std::string variable, std::string new_text);

	void setCombatMapDataAttributesValues();

public:

	SettingsMenu(sf::RenderWindow* window);
	~SettingsMenu();

	void handleUserInput();

	void loadFont(std::string file_name);

	void setupArrows();

	void setInitialCurrentVariable();

	void readSettingsFiles();

	void loadSettingsForNextEntity();

	void update(sf::Time deltaTime);

	void handleMenuInput();

	void render();

};