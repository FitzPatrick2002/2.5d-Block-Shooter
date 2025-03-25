import MenuTile;


MenuTile::MenuTile() {

}

MenuTile::~MenuTile() {

}

void MenuTile::init(sf::Vector2f pos, sf::Vector2f size) {
	this->setPosition(pos);
	this->setSize(size);

}

bool MenuTile::containsMouse(sf::Vector2f mouse_position) {

	return this->getGlobalBounds().contains(mouse_position);
}

void MenuTile::initText(sf::Font& f, std::string str) {
	this->text.setFont(f);
	this->text.setString(str);
	this->text.setFillColor(sf::Color::Red);
	this->text.setCharacterSize(20);

	/*sf::FloatRect bounds = this->text.getLocalBounds();
	this->text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top - bounds.height / 2.0f);

	float a = 0.5f;*/
	sf::Vector2f text_position = this->getPosition();

	this->text.setPosition(text_position);
}

void MenuTile::setText(std::string str) {
	this->text.setString(str);
}

sf::Text MenuTile::getText() {
	return this->text;
}

void MenuTile::setPos(sf::Vector2f pos) {
	this->setPosition(pos);

	// Update text position relative to the rectangle
}