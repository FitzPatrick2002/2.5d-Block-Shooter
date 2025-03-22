export module TextureManager;

import <SFML/Graphics.hpp>;

import <map>;
import <string>;

export class TextureManager {
private:

	std::map<std::string, sf::Texture> manager;

public:

	TextureManager();

	~TextureManager();

	void addTexture(std::string textureName, std::string fileName);

	sf::Texture& getTexture(std::string textureName);

};