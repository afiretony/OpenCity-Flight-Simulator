#ifndef MAP_H
#define MAP_H

#include <glm/glm.hpp>
#include <vector>
//#include <algorithm>
#include "obstacleavoid.h"

struct grid 
{
	// center coordinate
	glm::vec3 coord = { 0, 0, 0 };
	// model ID
	int id = 0;
	obstacle* block;
};

class Map{
private:
	int num_row;
	int num_col;
	const float grid_len = 0.08;
	string Path_to_Project;

public:
	Map(int row, int col, int maxId, const string path);
	~Map();
	// entire map of models
	std::vector<grid> grids_map;

	// random generated map
	void randMap(int maxId);
};

Map::Map(int row, int col, int maxId, const string path)
{
	num_row = row;
	num_col = col;
	Path_to_Project = path;
	randMap(maxId);
}

inline Map::~Map()
{
	for (auto& grid : grids_map) {
		delete grid.block;
	}
}

//inline void Map::regModel(const int m_id)
//{
//	grid m_grid;
//	m_grid.id = m_id;
//	grids_map.push_back(m_grid);
//}

//inline std::vector<grid>::iterator Map::findModel(const int m_id)
//{
//	//std::vector<grid>::iterator it;
//	const auto it = std::find_if(grids_map.begin(), grids_map.end(), 
//								[m_id](const grid& curr) {return curr.id == m_id; });
//	return it;
//}

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
		currGrid.block = new obstacle(4.0f, Path_to_Models, glm::vec3(1.0f, 1.0f, 1.0f), currGrid.coord);
	}
}
#endif