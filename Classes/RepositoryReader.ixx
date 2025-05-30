export module RepositoryReader;

import <vector>;

import <SFML/Graphics.hpp>;

export struct MapData {
	int width;
	int height;

	std::vector<float> heights;
};

export struct MapEnemyData {
	int total_number;
	int max_number;

	std::vector<sf::Vector2i> spawning_points;
};

struct MapFileData {
	MapData map_data;
	MapEnemyData enemies_map_data;
};

export class RepositoryReader {
private:

public:

	RepositoryReader() = default;
	~RepositoryReader() = default;

	/*
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

		std::regex map_regex("\\s*(-?\\d+\\.?\\d*)\\s*"); // Regex for reading rows of the map

		std::string line;
		int y = 0, x = 0;
		bool matched = false;
		while (std::getline(map_file, line)) {
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
		}

		map_file.close();

	}
	else {
		std::cerr << "Cannot open the file for the map\n";
	}
	
	*/

	void readMapFIle(std::string map_file) {

	}

};
