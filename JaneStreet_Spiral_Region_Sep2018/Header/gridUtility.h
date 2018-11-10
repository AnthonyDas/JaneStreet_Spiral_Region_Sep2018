#pragma once

#include <vector>

// Count shaded tiles
int count_shaded(const std::vector<bool> &);

// Prints out the grid
void print_grid(const std::vector<int> &, const std::vector<bool> &);

// Switches the shading (from off to on, or vice-versa) within a connected region
int switch_shading_within_connected_region(std::vector<bool> &, const int &, const bool &);

// Checks whether all shaded tile are connected via top, bottom, left or right 
// NB. Param copy passed by value
bool is_shaded_connected(std::vector<bool>);

// Calculate the product of the unshaded regions as per the example given
// NB. Param copy passed by value
int calculate_unshaded_regions_product(std::vector<bool>);

// Checks whether there are an equal number of shaded tiles per region
bool is_shaded_count_equal_between_areas(const std::vector<int> &, const std::map<int, int> &,
	const std::vector<bool> &, const int &);