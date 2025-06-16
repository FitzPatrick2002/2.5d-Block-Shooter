import CreditsMenu;
import GameManager;

import <sstream>;
import <fstream>;
import <iostream>;
import <regex>;

CreditsMenu::CreditsMenu(sf::RenderWindow* window) : GameState(window) {
	this->menuView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	this->loadFont("Game\\Resources\\Fonts\\Play-Bold");
	this->prepareTile();

}

CreditsMenu::~CreditsMenu() {

}

void CreditsMenu::handleUserInput() {

	while (this->window->pollEvent(this->stateEvent)) {

		switch (this->stateEvent.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:

			if (this->stateEvent.key.code == sf::Keyboard::M) {
				GameManager::getManager().createNewState(GameStateEnum::MainMenu, true); // Pop the current state from stack and create main menu.
				return;
			}

			break;
		case sf::Event::Resized:
			

			break;
		case sf::Event::MouseButtonPressed:
			
			return;

			break;
		default:
			break;
		}
	}

}

// 1. Read credits file
// 2. Count number of lines
// 3. Each line is 20 px
// 4. Resize tile 
// 5. Configure tile

void CreditsMenu::prepareTile() {
	std::ifstream file("Game\\Resources\\Credits\\Credits.txt");

	if (file.is_open()) {
		std::string line;
		std::string credits_message;

		// Count number of lines in ss
		std::regex end_line("(\\n)");
		std::regex blank_line("(^\\s*$)");
		std::smatch m;

		int blank_lines_number = 0;
		int total_lines = 0;
		while (std::getline(file, line)) {
			if (std::regex_match(line, m, blank_line))
				blank_lines_number++;
			else
				credits_message += (line + "\n");

			total_lines++;
		}
		int lines_with_content_num = total_lines - blank_lines_number;

		float window_w = static_cast<float>(this->window->getSize().x);
		float window_h= static_cast<float>(this->window->getSize().y);

		// Resize the tile
		this->message_tile.setSize(sf::Vector2f(window_w * 0.4f, 30.0f * lines_with_content_num));
		this->message_tile.setOutlineThickness(1.0f);
		this->message_tile.setOutlineColor(sf::Color(0, 255, 0, 255));
		this->message_tile.setFillColor(sf::Color::Black);
		this->message_tile.initText(this->tile_font, credits_message);
		this->message_tile.setPos(sf::Vector2f(window_w * 0.5f - this->message_tile.getSize().x / 2, window_h * 0.2f));

		file.close();
	}
	else {
		std::cerr << "Could not open the credits file\n";
	}

}

void CreditsMenu::loadFont(std::string file_name) {

	if (this->tile_font.loadFromFile(file_name + ".ttf")) {

	}
	else {
		std::cerr << "Cannot load font\n";
	}
}

void CreditsMenu::update(sf::Time deltaTime) {

	this->render();
	this->handleUserInput();
}

void CreditsMenu::render() {

	this->window->clear();

	this->window->draw(this->message_tile);
	this->window->draw(this->message_tile.getText());

	this->window->draw(this->top_tile);
	this->window->draw(this->top_tile.getText());
	
	this->window->display();
}