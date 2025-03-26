import GameMap;
import <iostream>;
import <random>;
import <ranges>;
import <algorithm>;

import linAlg;

import <thread>;
import <mutex>;

import <chrono>;
import<filesystem>;
import <regex>;
import <fstream>;
import <string>;
import <sstream>;
import <unordered_map>;

namespace fs = std::filesystem;

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

void GameMap::loadFromFile(std::string file_name) {
	std::ifstream map_file("Game\\Maps\\" + file_name + ".txt");

	auto read_width_height = [&, this]() {
		std::regex params_regex("([A-Za-z]+):\\s*(\\d+)");
		std::smatch m;

		std::map <std::string, int> params;

		// 1. Read the first lines -> should contain width and height
		// 2. Read the map matrix row by row
		//		2.1. After reading a row initilise data inside the map already

		std::string line;
		while (std::getline(map_file, line)) {
			if (std::regex_search(line, m, params_regex)) {
				params[m[1]] = std::atoi(m[2].str().c_str());
			}

			if (params["width"] and params["height"])
				break;
		}

		this->width = params["width"];
		this->height = params["height"];

		if (height <= 0 or width <= 0) {
			std::cerr << "Can't find dimensions of the map\n";
			return;
		}
		};

	auto create_tile = [&, this](int x, int y, float val) {
		sf::Vector3f pos(static_cast<float>(x), static_cast<float>(y), 0);
		sf::Vector3f dims(1.0f, 1.0f, val);

		this->ground[y * width + x].init(pos, dims);
		this->ground[y * width + x].setOutlineColor(sf::Color(0, 255, 0, 255));
		this->ground[y * height + x].setWallsColor(sf::Color(0, 0, 0, 255));
		this->ground[y * height + x].setWallsColor(sf::Color(255, 0, 0, 255), 0);
		this->ground[y * height + x].setWallsColor(sf::Color(0, 0, 255, 255), 1);
		this->ground[y * height + x].setWallsColor(sf::Color(125, 125, 0, 255), 2);

		};

	if (map_file.is_open()) {

		read_width_height();

		// Read the map matrix row by row and create 
		this->ground.clear();
		this->ground.resize(width * height);

		std::regex map_regex("\\s*(-?\\d+\\.?\\d*)\\s*");

		std::string line;
		int y = 0, x = 0;
		bool matched = false;
		while (std::getline(map_file, line)) {
			auto heights_begin = std::sregex_iterator(line.begin(), line.end(), map_regex);
			auto heights_end = std::sregex_iterator();

			matched = false;
			x = 0;
			for (std::regex_iterator i = heights_begin; i != heights_end; i++) {
				matched = true;
				std::smatch m = *i;

				float h = std::atof(m[1].str().c_str());
				create_tile(x, y, h);

				x++;
			}
			if(matched)
				y++;

			std::cout << line << "\n";
		}

		map_file.close();
	}
	else {
		std::cerr << "Cannot open the file for the map\n";
	}

}