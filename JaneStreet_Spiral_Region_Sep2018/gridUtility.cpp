#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm> // sort
#include <map>
#include <cmath> // sqrt

// Count shaded tiles
int count_shaded(const std::vector<bool> &grid) {
	int count = 0;
	for (std::vector<bool>::const_iterator it = grid.begin(); it != grid.end(); ++it) {
		if (*it == true) {
			++count;
		}
	}
	return count;
}

// Prints out the grid
void print_grid(const std::vector<int> &m_areas, const std::vector<bool> &grid) {
	const int size = (int)sqrt((double)grid.size());

	for (int i = 0; i < (int)grid.size(); ++i) {
		std::cout << std::setw(2) << std::setfill('0') << (grid[i] ? m_areas[i] : 0) << " ";
		if ((i + 1) % size == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

// Switches the shading (from off to on, or vice-versa) within a connected region
int switch_shading_within_connected_region(std::vector<bool> &grid, const int &index, const bool &target_state) {
	if (grid[index] == target_state) {
		return 0;
	}

	int count = 1;
	grid[index] = target_state;

	int size = (int)sqrt((double)grid.size());

	if (index % size != size - 1) {
		count += switch_shading_within_connected_region(grid, (index + 1), target_state);
	}
	if (index % size != 0) {
		count += switch_shading_within_connected_region(grid, (index - 1), target_state);
	}
	if (index >= size) {
		count += switch_shading_within_connected_region(grid, (index - size), target_state);
	}
	if (index < size * (size - 1)) {
		count += switch_shading_within_connected_region(grid, (index + size), target_state);
	}

	return count;
}

// Checks whether all shaded tile are connected via top, bottom, left or right 
// NB. Param copy passed by value
bool is_shaded_connected(std::vector<bool> copy) {
	// Find an index which is shaded as starting point
	for (size_t i = 0; i < copy.size(); ++i) {
		if (copy[i]) {
			switch_shading_within_connected_region(copy, (int)i, false);
			break;
		}
	}

	return count_shaded(copy) == 0;
}

// Calculate the product of the unshaded regions as per the example given
// NB. Param copy passed by value
int calculate_unshaded_regions_product(std::vector<bool> copy) {
	int product = 1;

	std::vector<int> factors;
	while (count_shaded(copy) != (int)copy.size()) {
		// Find an index which is not shaded as starting point
		for (size_t i = 0; i < copy.size(); ++i) {
			if (!copy[i]) {
				int count = switch_shading_within_connected_region(copy, (int)i, true);
				factors.push_back(count);
				product *= count;
				break;
			}
		}
	}
	std::sort(factors.begin(), factors.end());

	for (std::vector<int>::iterator it = factors.begin(); it != factors.end(); ++it) {
		std::cout << (it == factors.begin() ? "" : " * ") << *it;
	}
	std::cout << " = " << product << std::endl;
	return product;
}

// Checks whether there are an equal number of shaded tiles per region
bool is_shaded_count_equal_between_areas(const std::vector<int> &m_areas, const std::map<int, int> &m_areas_counts,
	const std::vector<bool> &grid, const int &shaded_per_area) {

	std::map<int, int> m;
	for (size_t i = 0; i < grid.size(); ++i) {
		if (grid[i]) {
			if (++m[m_areas[i]] > shaded_per_area) {
				return false;
			}
		}
	}

	if (!m.empty()) {
		const int count = m.begin()->second;
		for (std::map<int, int>::const_iterator it = m_areas_counts.begin(); it != m_areas_counts.end(); ++it) {
			if (m[it->first] != count) {
				return false;
			}
		}
	}
	return true;
}