#include <iostream>
#include <vector>

#include "SmallSpiralRegionSolver.cpp"
#include "LargeSpiralRegionSolver.cpp"

//
int main() {

	const std::vector<int> small_areas = 
	{4, 4, 4, 4, 4,
	4, 3, 3, 3, 4,
	4, 3, 1, 4, 4,
	4, 3, 1, 2, 4,
	3, 3, 3, 2, 2};
	
	SmallSpiralRegionSolver solver1(small_areas);

	const std::vector<int> large_areas = 
	{4,  4,  4,  5,  5, 11, 11, 20, 20, 20, 21, 21, 21, 21,
	 4,  4,  3,  5,  5,  5, 11, 11, 20, 20, 20, 20, 21, 21,
	 4,  3,  3,  5,  6,  6,  6, 11, 11, 19, 19, 20, 20, 21,
	 3,  3,  3,  2,  6,  7,  6,  6, 11, 19, 18, 18, 21, 21,
	 2,  3,  2,  2,  7,  7,  7, 14, 11, 19, 18, 18, 18, 11,
	 2,  3,  2,  7,  7,  8,  7, 14, 11, 19, 19, 19, 18, 11,
	 2,  2,  2,  2,  8,  8, 14, 14, 11, 13, 18, 18, 18, 11,
	 1,  2,  2,  8,  8, 14, 14, 11, 11, 13, 11, 11, 11, 11,
	 1,  1,  8,  8, 11, 11, 11, 11, 13, 13, 13, 17, 17, 11,
	 1,  9,  9, 10, 11, 12, 12, 12, 12, 13, 13, 13, 17, 11,
	 1,  1,  9, 10, 11, 11, 12, 15, 12, 16, 16, 17, 17, 11,
	 9,  9,  9, 10, 10, 11, 11, 15, 15, 15, 16, 16, 17, 11,
	 9,  10, 9, 10, 10, 10, 11, 11, 15, 15, 16, 16, 17, 11,
	 9,  10, 10, 10,10, 10, 10, 11, 11, 11, 11, 11, 11, 11};

	LargeSpiralRegionSolver solver2(large_areas);

	std::cout << "Finished - type something to quit" << std::endl;
	int dummy; std::cin >> dummy;
	return 0;
}
