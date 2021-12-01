#ifndef MAP_H
#define MAP_H

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "obstacleavoid.h"

struct grid 
{
	// center coordinate
	glm::vec3 coord = { 0, 0, 0 };
	// model ID
	int id = 0;
	obstacle* block;
	float block_height = 0.0f;
};

struct neighbour
{
	grid block;
	float dist;
};

class Map{
private:
	int num_row;
	int num_col;

	string Path_to_Project;

public:
	Map(int row, int col, const string path, int maxId = 10);
	~Map();

	// constants
	const float grid_len = 0.08;
	const float scale = 100.0f;

	// entire map of models
	std::vector<grid> grids_map;

	// random generated map
	void randMap(int maxId);
	// load pre-designed map
	void loadMap();

	// get neighbour info
	vector<neighbour> getNeighbour(const glm::vec3 pos);
};

Map::Map(int row, int col, const string path, int maxId)
{
	num_row = row;
	num_col = col;
	Path_to_Project = path;
	//randMap(maxId);
	loadMap();
}

inline Map::~Map()
{
	for (auto& grid : grids_map) {
		delete grid.block;
	}
}

inline void Map::randMap(int maxId)
{
	grids_map.resize(num_row * num_col);
	for (int i = 0; i < grids_map.size(); i++) {
		auto& currGrid = grids_map.at(i);
		int idx_row = floor(i / num_row);
		int idx_col = i % num_col;
		currGrid.coord.x = idx_col * grid_len + grid_len / 2;
		currGrid.coord.y = 0;
		currGrid.coord.z = idx_row * grid_len + grid_len / 2;
		currGrid.id = 0 + rand() % maxId;
		string Path_to_Models = Path_to_Project + "Glitter/Glitter/Model/City2/city" + to_string(currGrid.id + 1) + ".obj";
		currGrid.block = new obstacle(grid_len * scale * 0.8f, Path_to_Models, scale * glm::vec3(1.0f, 1.0f, 1.0f), currGrid.coord);
		if (currGrid.id > 2) currGrid.block_height = scale * currGrid.block->ObjectModel.height;
	}
}
inline void Map::loadMap()
{
	string inFileName;
	ifstream inFile;
	inFileName = Path_to_Project + "Glitter/Glitter/Model/City2/map.txt";

	inFile.open(inFileName);

	if (inFile.is_open()) {
		grids_map.clear();

		std::string wholeLineString;
		std::stringstream wholeLineStream;
		bool continueReading = true;
		int id;

		int i = 0;
		// go through file
		while (!inFile.eof() && continueReading) {
			// read the whole line
			getline(inFile, wholeLineString);
			wholeLineStream.str(wholeLineString.substr(0));
			
			wholeLineStream >> id;

			grid currGrid;
			currGrid.id = id;

			int idx_row = floor(i / num_row);
			int idx_col = i % num_col;
			currGrid.coord.x = idx_col * grid_len + grid_len / 2;
			currGrid.coord.y = 0;
			currGrid.coord.z = idx_row * grid_len + grid_len / 2;

			string Path_to_Models = Path_to_Project + "Glitter/Glitter/Model/City2/city" + to_string(currGrid.id + 1) + ".obj";
			currGrid.block = new obstacle(grid_len * scale * 0.8f, Path_to_Models, scale * glm::vec3(1.0f, 1.0f, 1.0f), currGrid.coord);
			if (currGrid.id > 2) currGrid.block_height = scale * currGrid.block->ObjectModel.height;
			grids_map.push_back(currGrid);

			i++;
			wholeLineStream.clear(); // get ready for next line
		}
		inFile.close();
	}
	else
		cout << "Was not able to open " << inFileName << " for input. " << endl;
}

inline vector<neighbour> Map::getNeighbour(const glm::vec3 pos)
{
	neighbour currNeigh;
	vector<neighbour> neighbourInfo;
	vector<neighbour> nearNeighbour;
	float h_dists;
	for (auto& grid : grids_map) {
		glm::vec2 obstacle_pos = glm::vec2(scale * grid.coord.x, scale * grid.coord.z);
		glm::vec2 uav_pos = glm::vec2(pos.x, pos.z);
		float h_dist = glm::length(obstacle_pos - uav_pos);
		//if (h_dist <= grid_len * scale) {
		currNeigh.block = grid;
		currNeigh.dist = h_dist;
		
		neighbourInfo.push_back(currNeigh);
			//std::cout << grid.id << "," << h_dist << "," << obstacle_pos.x << " " << obstacle_pos.y << "," << uav_pos.x << " " << uav_pos.y << std::endl;
		//}
	}
	sort(neighbourInfo.begin(), neighbourInfo.end(), [](const neighbour& a, const neighbour& b)->bool {return a.dist < b.dist; });
	for (int i = 0; i < 6; i++) {
		if (neighbourInfo.at(i).dist <= grid_len * scale) nearNeighbour.push_back(neighbourInfo.at(i));
	}
	return nearNeighbour;
}

#endif