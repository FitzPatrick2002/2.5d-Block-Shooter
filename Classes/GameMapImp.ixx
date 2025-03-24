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

	this->mapPool.stop();
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

void GameMap::generatePerlin() {

	std::vector<float> perlin_heights(this->height* this->width);
	int h = width + 1;
	int w = width + 1;

	std::vector<sf::Vector2f> gradient(h*w);
	std::vector<sf::Vector2f> inner_vectors(h*w);

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> angle_dist(0.f, 6.28f);
	std::uniform_real_distribution<float> pts_dist(0.0f, 1.0f);

	auto genRandVec = [&](std::uniform_real_distribution<float>& distr) {
		float alpha = distr(rng);
		return sf::Vector2f(cos(alpha), sin(alpha));
		};

	auto lerp = [&](float a, float b, float t) {
		return a + t * (b - a);
		};

	auto fade = [](float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
		};
	
	// 1.  Generate random gradient vectors
	for (int i = 0; i < gradient.size(); i++) {
		gradient[i] = genRandVec(angle_dist);
	}

	//2. Generate points -> vectors to the inside of the box 
	sf::Vector2f inner_vector;
	std::vector<float> dotProd(4);
	for (int i = 0; i < h - 1; i++) {
		for (int j = 0; j < w - 1; j++) {
			sf::Vector2f point(pts_dist(rng)*0.05f, pts_dist(rng)*0.05f);

			inner_vector = point;
			dotProd[0] = dotProduct(gradient[i * w + j], inner_vector);

			float u = fade(inner_vector.x);
			float v = fade(inner_vector.y);

			inner_vector = point - sf::Vector2f(1.0f, 0.0f);;
			dotProd[1] = dotProduct(gradient[i * w + (j + 1)], inner_vector);

			inner_vector = point - sf::Vector2f(1.0f, 1.0f);;
			dotProd[2] = dotProduct(gradient[(i  + 1) * w + (j + 1)], inner_vector);

			inner_vector = point - sf::Vector2f(0.0f, 1.0f);;
			dotProd[3] = dotProduct(gradient[(i + 1) * w + j], inner_vector);

			float mix_1 = lerp(dotProd[0], dotProd[1], u);
			float mix_2 = lerp(dotProd[3], dotProd[2], u);
			float combined = lerp(mix_1, mix_2, v);

			//float products_mean = mean<float>(dotProd);
			combined = combined * combined * (3 - 2 * combined);
			perlin_heights[i * width + j] = combined;
		}
	}

	sf::Vector3f boxDims(1.0f, 1.0f, 1.0f);
	sf::Vector3f pos; // Pos measured in tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pos.x = float(j) * boxDims.x;
			pos.y = float(i) * boxDims.y;
			//pos.z = (float(dist(rng)) + 0.5f) * boxDims.z * (dist(rng) == 1 ? (-1.0f) : 1.0f);
			//pos.z = (0.1f) * boxDims.z * (dist(rng) == 1 ? (-1.0f) : 1.0f);
			pos.z = 0.0f;
			boxDims.z = 300.0f*perlin_heights[i * width + j];
			this->ground[i * height + j].init(pos, boxDims);
			this->ground[i * height + j].setOutlineColor(sf::Color(0, 255, 0, 255));
			this->ground[i * height + j].setWallsColor(sf::Color(0, 0, 0, 255));
			this->ground[i * height + j].setWallsColor(sf::Color(255, 0, 0, 255), 0);
			this->ground[i * height + j].setWallsColor(sf::Color(0, 0, 255, 255), 1);
			this->ground[i * height + j].setWallsColor(sf::Color(125, 125, 0, 255), 2);
		}
	}


}

std::list<sf::Vector2i> drawWall(sf::Vector2i left_top, int len, int dir) {
	std::list<sf::Vector2i> pts;

	sf::Vector2i temp = left_top;
	switch (dir) {
	case 0: // Vertical
		for (int i = 0; i < len; i++) {
			temp.y -= 1;

			if (temp.y < 0)
				break;

			pts.push_back(temp);
		}

		break;
	case 1: // Horizontal
		for (int i = 0; i < len; i++) {
			temp.x -= 1;

			if (temp.x < 0)
				break;
			
			pts.push_back(temp);
		}

		break;
	}

	return pts;
}

void GameMap::generateMap() {

	//this->generatePerlin();

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 3);

	std::list<sf::Vector2i> walls_positions;
	sf::Vector2i partition_dims(width / 5, height / 5);

	std::uniform_int_distribution<std::mt19937::result_type> partition_dist_x(0, partition_dims.x - 1);
	std::uniform_int_distribution<std::mt19937::result_type> partition_dist_y(0, partition_dims.y - 1);

	sf::Vector2i current_position;
	sf::Vector2i random_point;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			current_position.x = i * partition_dims.x;
			current_position.y = i * partition_dims.y;

			random_point.x = partition_dist_x(rng);
			random_point.y = partition_dist_y(rng);

			random_point.x += current_position.x;
			random_point.y += current_position.y;

			walls_positions.splice(walls_positions.end(), drawWall(random_point, partition_dims.x/2, dist(rng) == 2));
		}
	}

	sf::Vector3f boxDims(1.0f, 1.0f, 1.0f);
	sf::Vector3f pos; // Pos measured in tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pos.x = float(j) * boxDims.x;
			pos.y = float(i) * boxDims.y;
			//pos.z = (float(dist(rng)) + 0.5f) * boxDims.z * (dist(rng) == 1 ? (-1.0f) : 1.0f);
			//pos.z = (0.1f) * boxDims.z * (dist(rng) == 1 ? (-1.0f) : 1.0f);
			pos.z = 0.0f;
			boxDims.z = 0.0f;//0.3f) * (float)(dist(rng));
			this->ground[i * height + j].init(pos, boxDims);
			this->ground[i * height + j].setOutlineColor(sf::Color(0, 255, 0, 255));
			this->ground[i * height + j].setWallsColor(sf::Color(0, 0, 0, 255));
			this->ground[i * height + j].setWallsColor(sf::Color(255, 0, 0, 255), 0);
			this->ground[i * height + j].setWallsColor(sf::Color(0, 0, 255, 255), 1);
			this->ground[i * height + j].setWallsColor(sf::Color(125, 125, 0, 255), 2);
		}
	}

	for (auto& e : walls_positions) {
		pos.x = e.x;
		pos.y = e.y;
		pos.z = 0;

		boxDims.z = 2.5f;

		this->ground[e.y * width + e.x].init(pos, boxDims);
		this->ground[e.y * width + e.x].setOutlineColor(sf::Color(0, 255, 0, 255));
		this->ground[e.y * width + e.x].setWallsColor(sf::Color(0, 0, 0, 255));
		this->ground[e.y * width + e.x].setWallsColor(sf::Color(255, 0, 0, 255), 0);
		this->ground[e.y * width + e.x].setWallsColor(sf::Color(0, 0, 255, 255), 1);
		this->ground[e.y * width + e.x].setWallsColor(sf::Color(125, 125, 0, 255), 2);

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

	/*for (int i = 0; i < this->height; i++) {
		//this->mapPool.queueJob(&GameMap::iterateOverWidth, i, this->width, playerPos, mousePos);
		this->mapPool.queueJob([this, i, playerPos, mousePos]() {
			this->iterateOverWidth(i, width, playerPos, mousePos);
			});
	}

	while (this->mapPool.busy()) {
		
	}*/
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
	std::ranges::for_each(this->ground, [&, this](MapBox& box) mutable {
		sf::Vector3f displacement;
		displacement = box.getWorldPos() + (-1.0f) * playerPos;
		float r = vectorLength(displacement);

		if (r < 25.0f) {
			sf::Vector2f disp_xy(displacement.x, displacement.y);
			float angle = dotProduct(disp_xy, mousePos) / (vectorLength(disp_xy)*vectorLength(mousePos));

			if(angle > 3.14f/4.0f)
				groundForDisplay.push_back(box);
		}

		});

}

bool GameMap::checkIfOnMap(sf::Vector2f pos) {

	return this->checkIfOnMap(sf::Vector2i((int)(pos.x), (int)(pos.y)));
}

bool GameMap::checkIfOnMap(sf::Vector2i pos) {
	if (pos.x < 0 or pos.x >= this->width)
		return false;

	if (pos.y < 0 or pos.y >= this->height)
		return false;

	return true;
}

bool GameMap::checkIfTileWalkable(sf::Vector2f pos) {
	
	return this->checkIfTileWalkable(sf::Vector2i((int)(pos.x), (int)(pos.y)));
}

bool GameMap::checkIfTileWalkable(sf::Vector2i pos) {
	if (not this->checkIfOnMap(pos))
		return false;

	if (this->ground[pos.y * width + pos.x].getDimensions().z > 0.5f)
		return false;

	return true;
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

int GameMap::getHeight() {
	return this->height;
}

int GameMap::getWidth() {
	return this->width;
}

MapBox GameMap::getTile(int i, int j) {

	if (i < 0 or i > this->height)
		std::cerr << "Map has dimesions: " << this->height << " " << this->width << "\n";

	if (j < 0 or j > this->width)
		std::cerr << "Map has dimesions: " << this->height << " " << this->width << "\n";;

	return this->ground[i * this->width + j];
}