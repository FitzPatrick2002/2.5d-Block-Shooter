export module SettingsMenu;

import GameState;
import MenuTile;

import <unordered_map>;
import <tuple>;
import <iostream>;
import <regex>;

// Comments DONE

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

// Settings menu contains settings, which can be nodified by player in order to adjust thr gameplay.
// Interface:
// All settings values are floats. 
// Value that we want to modify can be selected by clicking the tile with its name. 
// Tiles on the left side allow to modify the value of a setting.
// By clicking the top tile user can change the object to which the settings are applied.

// Settings are set in class CombatMapData in Math.h. It serves as a mediator object / resource holder. Though a bit poorly implemented...
// Settings files should be in directory relative to here the .exe file of the game is: Game/Resources/Settings/

export class SettingsMenu : public GameState {
private:
	sf::View menuView;
	//std::unordered_map <std::string, std::unordered_map<std::string, MenuTile>> settings; // The second element is the string that is the key to the modified value, that is stored in Math.ixx -> CombatMapDAta singleton class
	
	// Each object (like player, enemy, bullet, map...) has it's own set of attributes (settings)
	// They are displayed on tiles, organised one under another. 
	// The top tile contains the name of the object to which settings are referring to.
	// Settings are displayed below the top tile, to select a setting, user need to click on it.
	
	// Settings of each object are stored in a vector, these vectors are stored in an unordered_map.
	// Name of the object is the key to vector of it's settings. 
	std::unordered_map<std::string, std::vector<std::pair<std::string, MenuTile>>> settings_2;

	// "Arrows" are displayed on the left part of the screen.
	MenuTile arrow_up; // Tile used to increase value of the selected setting.
	MenuTile arrow_down; // Tile used to decrese the value of the selected setting.

	sf::Font file_font;

	std::string current_entity; // Name of the currently modified object.
	std::string current_entity_variable; //TO DO

private:

	void addObjectToSettings_2(std::string object);

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