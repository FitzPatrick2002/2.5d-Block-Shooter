import Animation;

Animation::Animation() {
	this->currentAnimation = 0;

	this->totalTime = 0.0f;
	this->switchTime = 0.0f;
}

Animation::Animation(sf::Texture* texture, std::vector<int>& imCount, float tbf) {
	this->resetTexture(texture, imCount, tbf);

}

Animation::~Animation() {

}

void Animation::resetTexture(sf::Texture* texture, std::vector<int>& imCount, float tbf) {

	this->imageCount = imCount;
	this->switchTime = tbf;

	this->totalTime = 0.0f;
	currentImage.x = 0;

	int animationPictureWidth = *max_element(imCount.begin(), imCount.end());

	uvRect.width = texture->getSize().x / float(animationPictureWidth);
	uvRect.height = texture->getSize().y / float(imCount.size());
}

void Animation::update(sf::Time elapsedTime) {

	currentImage.y = currentAnimation;
	totalTime += elapsedTime.asSeconds();

	if (totalTime >= switchTime) {
		totalTime = 0.0f;

		currentImage.x = (currentImage.x + 1) % imageCount[currentAnimation];
	}

	uvRect.left = currentImage.x * uvRect.width;
	uvRect.top = currentImage.y * uvRect.height;
}

void Animation::setAnimationNum(int i) {
	if (i < 0 || i >= imageCount.size()) {
		std::cerr << "i is out of range: [" << 0 << " : " << imageCount.size() << "]\n";
		return;
	}

	this->currentAnimation = i;
}