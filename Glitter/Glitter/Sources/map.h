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

class Map{
private:
	int num_row;
	int num_col;

	const float grid_len = 0.08;
	const float scale = 100.0f;

	string Path_to_Project;

public:
	Map(int row, int col, int maxId, const string path);
	~Map();
	// entire map of models
	std::vector<grid> grids_map;

	// random generated map
	void randMap(int maxId);
	// load pre-designed map
	void loadMap(int maxId);

	// get neighbour grid
	vector<grid> getNeighbour(const glm::vec3 pos);
};

Map::Map(int row, int col, int maxId, const string path)
{
	num_row = row;
	num_col = col;
	Path_to_Project = path;
	//randMap(maxId);
	loadMap(maxId);
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
		currGrid.block = new obstacle(4.0f, Path_to_Models, scale*glm::vec3(1.0f, 1.0f, 1.0f), currGrid.coord);
		currGrid.block_height = scale * currGrid.block->ObjectModel.height;
	}
}
inline void Map::loadMap(int maxId)
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
			currGrid.block = new obstacle(4.0f, Path_to_Models, glm::vec3(100.0f, 100.0f, 100.0f), currGrid.coord);
			grids_map.push_back(currGrid);
			currGrid.block_height = 100.0f * currGrid.block->ObjectModel.height;


			i++;
			wholeLineStream.clear(); // get ready for next line
		}
		inFile.close();
	}
	else
		cout << "Was not able to open " << inFileName << " for input. " << endl;
}

inline vector<grid> Map::getNeighbour(const glm::vec3 pos)
{
	for (auto& grid : grids_map) {

	}
	return vector <grid>();
}

#endif