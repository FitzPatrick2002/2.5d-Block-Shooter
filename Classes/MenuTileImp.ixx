import MenuTile;

// Comments DONE

// Constructor, empty
MenuTile::MenuTile() {

}

// Destructor, empty
MenuTile::~MenuTile() {

}

// Durng initialisation, set the position and size of the tile.

void MenuTile::init(sf::Vector2f pos, sf::Vector2f size) {
	this->setPosition(pos);
	this->setSize(size);

}

// Check global bounds if the rectangle shape, which creates the tile, contains the mouse.

bool MenuTile::containsMouse(sf::Vector2f mouse_position) {
	return this->getGlobalBounds().contains(mouse_position);

}

// Initialise text. 
// Text is always red.
// Character size is 20.
// Position of the text is same as the rectangle shape.
// f - font 
// str - text stored by the tile.

void MenuTile::initText(sf::Font& f, std::string str) {
	this->text.setFont(f);
	this->text.setString(str);
	this->text.setFillColor(sf::Color::Red);
	this->text.setCharacterSize(20);

	sf::Vector2f text_position = this->getPosition();
	this->text.setPosition(text_position);
}

// Sets the text, stored by the tile.
void MenuTile::setText(std::string str) {
	this->text.setString(str);
}

// Returns text, stored by the tile.
sf::Text MenuTile::getText() {
	return this->text;
}

// Sets the position of the tile with the text inside. 
void MenuTile::setPos(sf::Vector2f pos) {
	this->setPosition(pos);
	this->text.setPosition(pos);
	// Update text position relative to the rectangle
}