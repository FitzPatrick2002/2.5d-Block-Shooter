import GameMap;

import <iostream>;
import <random>;
import <ranges>;
import <algorithm>;
import <atomic>;

import linAlg;

import <thread>;
import <mutex>;

import <future>;
import <vector>;
import <chrono>;
import<filesystem>;
import <regex>;
import <fstream>;
import <string>;
import <sstream>;
import <unordered_map>;
import <algorithm>;
import <set>;


namespace fs = std::filesystem;

GameMap::GameMap() {
	//mapPool.start();
}

GameMap::~GameMap() {

	//this->mapPool.stop();
}

void GameMap::init(int w, int h, TextureManager* tM) {
	this->width = w;
	this->height = h;

	this->textureManager = tM;

	this->ground.resize(w * h);

	this->generateMap();
}

void GameMap::initChunks() {

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

			walls_positions.splice(walls_positions.end(), drawWall(random_point, partition_dims.x / 2, dist(rng) == 2));
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

/*
std::mutex mu;
void GameMap::iterateOverWidth(int y, int width, sf::Vector3f playerPos, sf::Vector2f mousePos) {
	std::vector<MapBox> localBoxes;

	auto subrange = this->ground
		| std::ranges::views::drop(y * this->width)
		| std::ranges::views::take(width);

	std::ranges::for_each(subrange, [&, this](MapBox& box) mutable {
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
*/

/*
// Mouse position in tiles pls
void GameMap::setFOVraycasting(sf::Vector2f player_pos, sf::Vector2f mouse_position) {
	sf::Vector2f delta_pos = mouse_position - player_pos;

	float cone_angle = atan2(delta_pos.y, delta_pos.x);
	float delta_angle = cone_angle / 100.0f;
	float delta_r = 0.1f;
	sf::Vector2f versor = makeVersor(delta_pos);

	std::set<sf::Vector3f> visiblePositions;

	for (int i = -49; i < 50; i++) {
		float r = 0;
		float current_angle = cone_angle + i*delta_angle;
		sf::Vector2f versor_temp = rotateVector(versor, current_angle);

		// Put these whiles to the thread I mean queue 100 calls to the threadPool
		while (r < 20.0f) {
			sf::Vector2f current_pos = r * versor_temp;
			if (this->checkIfTileWalkable(current_pos)) {
				
				this->groundForDisplayRayCasting.insert(current_pos);
			}
			else
				break;

			r += delta_r;
		}
	}
	
}
*/

// 1. Calc vector from player screen pos (0,0) to mouse position relative to the center of the screen
// 2. Calc vector from box to the players position (in the second variany just get the screen position of the box)
void GameMap::setPlayerFOV(sf::Vector3f playerPos, sf::Vector2f mousePos) {

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

	// Basic option
	/*
	 this->groundForDisplay.clear();
	std::ranges::for_each(this->ground, [&, this](MapBox& box) mutable { // ------------------------------------- !!! Uncomment this. It's the fov
		sf::Vector3f displacement;
		displacement = box.getWorldPos() - playerPos;
		float r = vectorLength(displacement);

		if (r < 25.0f) {
			sf::Vector2f disp_xy(displacement.x, displacement.y);
			float angle = dotProduct(disp_xy, mousePos) / (vectorLength(disp_xy) * vectorLength(mousePos));

			if (angle > 3.14f / 4.0f)
				groundForDisplay.push_back(box);
		}

		});
	*/
	// End of basic option

	// Raycasting option
	//this->groundForDisplayRayCasting.clear();
	//this->setFOVraycasting(sf::Vector2f(playerPos.x, playerPos.y), mousePos);

	
	float theta = atan2(mousePos.y, mousePos.x);
	std::cout << "Theta: " << theta << "\n";
	float alpha = 3.14f / 4.0f;

	// New bresenham raycasting
	this->ground_to_display_s = Geometry::rayCastingBresenhamParallel(
		[this](int x, int y) -> bool {return this->checkIfTileWalkable(sf::Vector2i(x, y)); }, width, height,
		V3_to_2i(playerPos), 25.0f, theta, alpha);
	
	
}

void GameMap::setRenderOrder(std::vector<Entity*> &elems_to_draw) {
	/*
	auto comp = [](const MapBox& a, const MapBox& b) -> bool {
		sf::Vector3f va = a.getWorldPos();
		sf::Vector3f vb = b.getWorldPos();

		float sum_a = va.x + va.y - va.z; // + va.x;
		float sum_b = vb.x + vb.y - vb.z; // + vb.x;

		return sum_a < sum_b;
		};

	std::ranges::sort(groundForDisplay, comp); // Why an error? And this works regrardless of underline?
	*/

	elems_to_draw.clear();
	elems_to_draw.resize(0);
	elems_to_draw.resize(ground_to_display_s.size());

	int i = 0;
	for (auto & pos : ground_to_display_s) {
		elems_to_draw[i] = &(this->ground[pos.y * width + pos.x]);
		i++;
	}

	auto comp_pts = [](const Entity* p1, const Entity* p2) {

		float sum_1 = p1->getWorld_XY().x + p1->getWorld_XY().y; // -.z
		float sum_2 = p2->getWorld_XY().x + p2->getWorld_XY().y; // -.z

		return sum_1 < sum_2;
		};

	std::ranges::sort(elems_to_draw, comp_pts);
	
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

// Improved rendering by batching

void GameMap::batchBoxes(sf::VertexArray& lines_array, sf::VertexArray &quads_array) {

	auto comp = [](const MapBox& a, const MapBox& b) -> bool {
		sf::Vector3f va = a.getWorldPos();
		sf::Vector3f vb = b.getWorldPos();

		float sum_a = va.x + va.y - va.z; // + va.x;
		float sum_b = vb.x + vb.y - vb.z; // + vb.x;

		return sum_a < sum_b;
		};

	std::vector<MapBox> sorted_boxes;
	sorted_boxes = this->ground;

	std::ranges::sort(sorted_boxes, comp);

	for (auto& b : sorted_boxes) {
		b.batchLines(lines_array);
		b.batchQuads(quads_array);
	}
}

void GameMap::render(sf::RenderWindow* w) {

	
	//for (auto& e : this->ground)
	//	e.render(w);

	// Basic render used with:
	// void GameMap::setRenderOrder() {
	// void GameMap::setPlayerFOV(sf::Vector3f playerPos, sf::Vector2f mousePos) {
	
	// Basic option
	/*
	for (auto& e : this->groundForDisplay)
		e.render(w);
	*/

	/*
	for (auto& e : this->ground_to_display_s)
		this->ground[e.y * width + e.x].render(w);
	*/

	/*for (auto& e : this->displayed_objects) // This was used with the bresenhamRayCastingParallel
		e->render(w);*/
	

	// rendering with the use of raycasting
	//for (auto& pos : this->groundForDisplayRayCasting) {
	//	this->ground[this->width * pos.y + pos.x].render(w);
	//}

}

int GameMap::getHeight() {
	return this->height;
}

int GameMap::getWidth() {
	return this->width;
}

sf::Vector2i GameMap::getChunksAmount() {
	return this->chunks_amount;
}

MapBox& GameMap::getTile(int x, int y) {

	if (y < 0 or y >= this->height)
		std::cerr << "Map has dimesions: " << this->height << " " << this->width << "\n";

	if (x < 0 or x >= this->width)
		std::cerr << "Map has dimesions: " << this->height << " " << this->width << "\n";;

	return this->ground[y * this->width + x];
}

void GameMap::loadFromFile(std::string file_name) {
	std::ifstream map_file("Game\\Maps\\" + file_name + ".txt");

	auto read_width_height = [&, this]() {
		std::regex params_regex("([A-Za-z ]+):\\s*(\\d+)");
		std::smatch m;

		std::map <std::string, int> params;

		// 1. Read the first lines -> should contain width and height
		// 2. Read the map matrix row by row
		//		2.1. After reading a row initilise data inside the map already

		// Read width and height
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

	auto create_tile = [this](int x, int y, float val) {
		sf::Vector3f pos(static_cast<float>(x), static_cast<float>(y), 0);
		sf::Vector3f dims(1.0f, 1.0f, val*0.5f);

		
		this->ground[y * width + x].init(pos, dims);
		this->ground[y * width + x].setOutlineColor(sf::Color(0, 255, 0, 255));
		this->ground[y * width + x].setWallsColor(sf::Color(0, 0, 0, 255));
		this->ground[y * width + x].setWallsColor(sf::Color(255, 0, 0, 255), 0); // ERROR - 15x20 map vector subscript out of range
		this->ground[y * width + x].setWallsColor(sf::Color(0, 0, 255, 255), 1);
		this->ground[y * width + x].setWallsColor(sf::Color(125, 125, 0, 255), 2);
		};

	auto create_map_row = [create_tile, this](std::string row_data, int row_num) {
		std::regex map_regex("\\s*(-?\\d+\\.?\\d*)\\s*"); // Regex for reading rows of the map
		std::smatch m;

		std::sregex_iterator begin = std::sregex_iterator(row_data.begin(), row_data.end(), map_regex);
		std::sregex_iterator end = std::sregex_iterator();

		int i = 0;
		for (std::sregex_iterator itr = begin; itr != end; itr++) {
			m = *itr;

			float h = std::atof(m[1].str().c_str());

			create_tile(i, row_num, h);
			i++;
		}
	};

	if (map_file.is_open()) {

		read_width_height(); // Width and height should be provided at the beginning of the file

		// Read the map matrix row by row and create the map
		this->ground.clear();
		this->ground.resize(width * height);

		std::regex map_regex("\\s*(-?\\d+\\.?\\d*)\\s*"); // Regex for reading rows of the map
		std::smatch m;

		std::string line;
		int y = 0;
		int x = 0;

		std::vector<std::future<void>> async_row_building(this->height);

		// The map matrix should be all together without any blank lines in between.
		int map_rows_count = 0;
		//while (std::getline(map_file, line) and map_rows_count < this->height) { // Old
		while (std::getline(map_file, line) and y < this->height) {
			// Async plan:
			// 1. Getline 
			// 2. Let the async build this line
			// 3. Move to the next line, repeat till enitre map matrix is read

			async_row_building[y] = std::async(create_map_row, line, y);
			//create_map_row(line, y, matched);
			//if(matched)
			//	y++;

			if (std::regex_search(line, m, map_regex))
				y++;
		}
		// Wait for asyncs to finish
		for (auto& e : async_row_building)
			e.get();
		
		map_file.close();
	}
	else {
		std::cerr << "Cannot open the file for the map\n";
	}

}

/*

void GameMap::loadFromFile(std::string file_name) {
	std::ifstream map_file("Game\\Maps\\" + file_name + ".txt");

	auto read_width_height = [&, this]() {
		std::regex params_regex("([A-Za-z ]+):\\s*(\\d+)");
		std::smatch m;

		std::map <std::string, int> params;

		// 1. Read the first lines -> should contain width and height
		// 2. Read the map matrix row by row
		//		2.1. After reading a row initilise data inside the map already

		// Read width and height
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
		sf::Vector3f dims(1.0f, 1.0f, val*0.5f);


		this->ground[y * width + x].init(pos, dims);
		this->ground[y * width + x].setOutlineColor(sf::Color(0, 255, 0, 255));
		this->ground[y * width + x].setWallsColor(sf::Color(0, 0, 0, 255));
		this->ground[y * width + x].setWallsColor(sf::Color(255, 0, 0, 255), 0); // ERROR - 15x20 map vector subscript out of range
		this->ground[y * width + x].setWallsColor(sf::Color(0, 0, 255, 255), 1);
		this->ground[y * width + x].setWallsColor(sf::Color(125, 125, 0, 255), 2);
		};

	auto create_map_row = [&, this](std::string row_data, int row_num) {

		};

	if (map_file.is_open()) {

		read_width_height(); // Width and height should be provided at the beginning of the file

		// Read the map matrix row by row and create the map
		this->ground.clear();
		this->ground.resize(width * height);

		std::regex map_regex("\\s*(-?\\d+\\.?\\d*)\\s*"); // Regex for reading rows of the map
		std::smatch m;

		std::string line;
		int y = 0, x = 0;
		bool matched = false;

		// The map matrix should be all together without any blank lines in between.
		int map_rows_count = 0;
		//while (std::getline(map_file, line) and map_rows_count < this->height) { // Old
		while (std::getline(map_file, line) and y < this->height) {

			// Async plan:
			// 1. Getline
			// 2. Check if map is divisible by 5 or by 10
			// 3. If by 10 then divide the width of a row by 10, otherwise by 5
			// 4. Create n = width / 10 (or 5) strings
			// 5. Parse each od them with a regex iterator separately and fill the map.


			auto heights_begin = std::sregex_iterator(line.begin(), line.end(), map_regex);
			auto heights_end = std::sregex_iterator();

			// Iterate thorough the line and find all occurences of the patter -> reading heights of cells)

			matched = false;
			x = 0;
			for (std::regex_iterator i = heights_begin; i != heights_end; i++) {
				matched = true;
				std::smatch m = *i;

				float h = std::atof(m[1].str().c_str());
				create_tile(x, y, h);

				x++;
			}
			if (matched)
				y++;

			std::cout << line << "\n";
			map_rows_count++;

		}

		map_file.close();

	}
	else {
		std::cerr << "Cannot open the file for the map\n";
	}

}
*/