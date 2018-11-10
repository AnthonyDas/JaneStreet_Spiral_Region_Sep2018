#pragma once

#include <vector>
#include <map>

// Class to solve the main problem
class largeSpiralRegionSolver {

	const std::vector<int> m_areas;

	std::map<int, int> m_areas_counts;

	int min_product;

public:

	largeSpiralRegionSolver(const std::vector<int> &);

private:

	// Populated a full size grid based on our working half length grid representing a single quadrant
	void populate_full_grid(std::vector<bool> &, const int &, const std::vector<bool> &) const;

	// Brute force shading in (remaining) cells adhering to what cells we are given or can deduce 
	void shade_cells(std::vector<bool> &, const int &, const int &);

};
