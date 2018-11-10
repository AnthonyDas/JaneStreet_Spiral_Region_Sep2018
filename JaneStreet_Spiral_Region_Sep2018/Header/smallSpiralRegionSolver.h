#pragma once

#include <vector>
#include <map>

// Class to solve the example problem
class smallSpiralRegionSolver {

	const std::vector<int> m_areas;

	std::map<int, int> m_areas_counts;

	int min_product;

public:

	smallSpiralRegionSolver(const std::vector<int> &);

private:

	// Main processing function
	void recursive_shade_cells(const int &, const int &, std::vector<bool> &,
		const int &, const int &, const int &);

};
