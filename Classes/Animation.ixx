export module Animation;

import <SFML/Graphics.hpp>;
import <vector>;
import <algorithm>;
import <iostream>;

export class Animation {
private:

	std::vector<int> imageCount; // [i] = l. i -> row (animation number), l -> animation length measured in pictures
	int currentAnimation;

	sf::Vector2u currentImage; // Position of the current image (counted in images from top to bottom, from left to right)

	float totalTime;
	float switchTime;

public:

	sf::IntRect uvRect;

public:

	Animation();

	Animation(sf::Texture* texture, std::vector<int>& imCount, float tbf);

	~Animation();

	void resetTexture(sf::Texture* texture, std::vector<int>& imCount, float tbf);

	void update(sf::Time elapsedTime);

	void setAnimationNum(int i);

};