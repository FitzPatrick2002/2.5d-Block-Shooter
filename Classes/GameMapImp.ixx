import GameMap;
import <iostream>;
import <random>;
import <ranges>;
import <algorithm>;

import linAlg;

import <thread>;
import <mutex>;

import <chrono>;

GameMap::GameMap() {
	mapPool.start();
}

GameMap::~GameMap() {

}

void GameMap::init(int w, int h, TextureManager* tM) {
	this->width = w;
	this->height = h;

	this->textureManager = tM;

	// Apparently only three points are deeded. 
	// The rest will be filled by the consecutive cells

	this->ground.resize(w * h);

	/*sf::Vector3f pos; // Pos measured in tiles
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			pos.x = float(j) + 20.0f;
			pos.y = float(i);
			pos.z = float(5-i) * float(5-j) / 10.0f;
			this->ground[i * h + j].init(pos);
		}
	}*/

	this->generateMap();

	/*
	std::vector<sf::Vector3f> pts = {
		sf::Vector3f(0.0f,0.0f,0.0f),
		sf::Vector3f(1.0f,0.0f,0.0f),
		sf::Vector3f(1.0f,1.0f,0.0f),
		sf::Vector3f(0.0f,1.0f,0.0f),
		sf::Vector3f(0.0f,0.0f,0.0f)
	};

	sf::Vector3f pos; // Pos measured in tiles
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			pos.x = float(j) + 20.0f;
			pos.y = float(i);
			this->ground[i * h + j].reset(sf::LinesStrip, pos, pts, scrTileSize);
		}
	}*/
	this->temp.init(sf::Vector3f(0, 0, 0), sf::Vector3f(0.1f, 0.1f, 0.1f));
	this->temp.setOutlineColor(sf::Color::White);
	this->temp.setWallsColor(sf::Color::Yellow);
}

void GameMap::generateMap() {

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 10);
	//std::uniform_int_distribution<std::mt19937::result_type> hei(-1, 1);

	sf::Vector3f boxDims(1.0f, 1.0f, 1.0f);
	sf::Vector3f pos; // Pos measured in tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pos.x = float(j) * boxDims.x;
			pos.y = float(i) * boxDims.y;
			//pos.z = (float(dist(rng)) + 0.5f) * boxDims.z * (dist(rng) == 1 ? (-1.0f) : 1.0f);
			//pos.z = (0.1f) * boxDims.z * (dist(rng) == 1 ? (-1.0f) : 1.0f);
			pos.z = 0.0f;
			boxDims.z = (0.1f) * (float)(dist(rng));
			this->ground[i * height + j].init(pos, boxDims);
			this->ground[i * height + j].setOutlineColor(sf::Color(0, 255, 0, 255));
			this->ground[i * height + j].setWallsColor(sf::Color(0, 0, 0, 255));
			this->ground[i * height + j].setWallsColor(sf::Color(255, 0, 0, 255), 0);
			this->ground[i * height + j].setWallsColor(sf::Color(0, 0, 255, 255), 1);
			this->ground[i * height + j].setWallsColor(sf::Color(125, 125, 0, 255), 2);

			//std::cout << "Box pos: " << this->ground[i * height + j].getWorldPos() << "\n";
		}
	}

}

std::mutex mu;
void GameMap::iterateOverWidth(int y, int width, sf::Vector3f playerPos, sf::Vector2f mousePos) {
	std::vector<MapBox> localBoxes;

	auto subrange = this->ground
		|	std::ranges::views::drop(y * this->width)
		| std::ranges::views::take(width);

	std::ranges::for_each(subrange, [&, this](MapBox & box) mutable {
		sf::Vector3f displacement;
		displacement = box.getWorldPos() + (-1.0f) * playerPos;
		float r = vectorLength(displacement);

		if (r < 10.0f) {
			sf::Vector2f disp_xy(displacement.x, displacement.y);
			float angle = dotProduct(disp_xy, mousePos) / (vectorLength(disp_xy) * vectorLength(mousePos));

			if (angle > 3.14f / 4.0f) {

				localBoxes.push_back(box);
			}
		}
		});

	std::lock_guard<std::mutex> lock(mu);
	this->groundForDisplay.insert(this->groundForDisplay.end(), localBoxes.begin(), localBoxes.end());
}

// 1. Calc vector from player screen pos (0,0) to mouse position relative to the center of the screen
// 2. Calc vector from box to the players position (in the second variany just get the screen position of the box)
void GameMap::setPlayerFOV(sf::Vector3f playerPos, sf::Vector2f mousePos) {
	this->groundForDisplay.clear();

	for (int i = 0; i < this->height; i++) {
		//this->mapPool.queueJob(&GameMap::iterateOverWidth, i, this->width, playerPos, mousePos);
		this->mapPool.queueJob([this, i, playerPos, mousePos]() {
			this->iterateOverWidth(i, width, playerPos, mousePos);
			});
	}

	while (this->mapPool.busy()) {
		
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	/*std::vector<std::shared_ptr<std::thread>> threads;
	threads.resize(this->height);

	int i = 0;
	for (auto& ptr : threads) {
		ptr = std::make_shared<std::thread>([&, this, i]() {iterateOverWidth(i, this->width, playerPos, mousePos); });
		i++;
	}

	for (auto& ptr : threads) {
		ptr->join();
	}*/
	/*std::ranges::for_each(this->ground, [&, this](MapBox& box) mutable {
		sf::Vector3f displacement;
		displacement = box.getWorldPos() + (-1.0f) * playerPos;
		float r = vectorLength(displacement);

		if (r < 10.0f) {
			sf::Vector2f disp_xy(displacement.x, displacement.y);
			float angle = dotProduct(disp_xy, mousePos) / (vectorLength(disp_xy)*vectorLength(mousePos));

			if(angle > 3.14f/4.0f)
				groundForDisplay.push_back(box);
		}

		});*/

}

void GameMap::setRenderOrder() {
	//this->groundForDisplay.reserve(10);

//	sf::Vector2i playerPos(std::ceil(pPos.x), std::ceil(pPos.y));
//	sf::Vector2i tilePos;

	//std::cout << "CurrentPLayer : " << this->ground[(int)pPos.y * this->height + (int)pPos.x].getWorldPos() << "\n";

	// Addall tiles from a circle
	/*
	std::ranges::for_each(this->ground, [&, this](MapBox& box) mutable {
		
		sf::Vector3f displacement;
		displacement = box.getWorldPos() + (-1.0f) * pPos;
		float r = vectorLength(displacement);
		
		if (r < 10.0f)
			groundForDisplay.push_back(box);
		
		//std::cout << "pos: " << box.getWorldPos() << "\n";
		
		});
	*/
	
	/*for (int i = -4; i < 3; i++) {
		for (int j = -4; j < 3; j++) {
			tilePos.y = std::max(playerPos.y + i, 0);
			tilePos.x = std::max(playerPos.x + j, 0);

			//std::cout << "box pos: " << this->ground[tilePos.y * height + tilePos.x].getWorldPos() << "\n";

			this->groundForDisplay.push_back(this->ground[tilePos.y * height + tilePos.x]);
		}
	}*/

	auto comp = [](const MapBox& a, const MapBox& b) -> bool {
		sf::Vector3f va = a.getWorldPos();
		sf::Vector3f vb = b.getWorldPos();

		float sum_a = va.x + va.y -va.z; // + va.x;
		float sum_b = vb.x + vb.y - vb.z; // + vb.x;
	
		return sum_a < sum_b;
	};

	std::ranges::sort(groundForDisplay, comp); // Why an error? And this works regrardless of underline?

}

void GameMap::render(sf::RenderWindow* w) {

	/*for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			this->ground[i * height + j].render(w);
		}
	}*/

	//for (auto& e : this->ground)
	//	e.render(w);

	//this->temp.render(w);

	for (auto& e : this->groundForDisplay)
		e.render(w);
}

MapBox GameMap::getTile(int i, int j) {
	return this->ground[i * this->width + j];
}