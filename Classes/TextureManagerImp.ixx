import TextureManager;
import <iostream>;

TextureManager::TextureManager() {
	
}

TextureManager::~TextureManager() {

}

void TextureManager::addTexture(std::string textureName, std::string fileName) {

	// There already exist a texture with this name
	if (manager.find(textureName) != manager.end()) {
		std::cerr << "Texture with such name: " << textureName << " already exist.\n";
		return;
	}

	manager[textureName].loadFromFile(fileName);
}

sf::Texture& TextureManager::getTexture(std::string textureName) {
	sf::Texture t;

	if (manager.find(textureName) != manager.end())
		return manager[textureName];

	return t;
}