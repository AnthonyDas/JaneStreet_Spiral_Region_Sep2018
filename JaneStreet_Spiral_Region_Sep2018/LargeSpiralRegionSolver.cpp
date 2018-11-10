#include <iostream>
#include <vector>
#include <map>
#include <cmath> // sqrt
#include "largeSpiralRegionSolver.h"
#include "gridUtility.h"


largeSpiralRegionSolver::largeSpiralRegionSolver(const std::vector<int> &areas) : m_areas(areas), min_product(INT_MAX) {

	const int areas_side_len = (int)sqrt((double)areas.size());

	for (std::vector<int>::const_iterator it = areas.begin(); it != areas.end(); ++it) {
		++m_areas_counts[*it];
	}

	int smallest_area_count = INT_MAX;
	for (std::map<int, int>::const_iterator it = m_areas_counts.begin(); it != m_areas_counts.end(); ++it) {
		if (it->second < smallest_area_count) {
			smallest_area_count = it->second;
		}
	}

	for (int shaded_per_area = 1; shaded_per_area <= smallest_area_count; ++shaded_per_area) {
		int shaded_count = shaded_per_area * (int)m_areas_counts.size();
		std::cout << "Finding solutions for shaded count = " << shaded_count << std::endl;

		/*if (shaded_count % 4 == 0) {

			From thought and paper and pen calculations, the shaded cells must lie within
			a 13 x 13 grid placed in the top left. Further, 105 cells must be shaded.
			1 in the very centre and then 26 in each of the four quadrants.

		}
		// else if ((shaded_count - 1) % 4 == 0) {*/
		if (shaded_count == 105) {

			int half_size = areas_side_len / 2;
			std::vector<bool> grid((half_size * (half_size - 1)), false);

			shade_cells(grid, shaded_per_area, areas_side_len);
		}
	}

	std::cout << "Min Product: " << min_product << std::endl;
}



// Populated a full size grid based on our working half length grid representing a single quadrant
void largeSpiralRegionSolver::populate_full_grid(std::vector<bool> &full_grid, const int &areas_side_len, const std::vector<bool> &grid) const {
	full_grid[(14 * 6) + 6] = true; // Centre

	int half_size = areas_side_len / 2;

	for (int i = 0; i < (int)grid.size(); ++i) {
		if (grid[i]) {

			int x = i % half_size;
			int y = i / half_size;

			// Top left quadrant
			full_grid[x + (y * areas_side_len)] = true;

			// Bottom left quadrant
			full_grid[y + ((areas_side_len - 2 - x) * areas_side_len)] = true;

			// Bottom right quadrant
			full_grid[(areas_side_len - 2 - x) + ((areas_side_len - 2 - y) * areas_side_len)] = true;

			// Top right quadrant
			full_grid[(areas_side_len - 2 - y) + (x * areas_side_len)] = true;
		}
	}
}

// Brute force shading in (remaining) cells adhering to what cells we are given or can deduce 
void largeSpiralRegionSolver::shade_cells(std::vector<bool> &grid, const int &shaded_per_area, const int &areas_side_len) {

	// Deduced from area 21
	grid[0] = grid[1] = grid[3] = grid[7] = grid[14] = true;

	// Centre cell to be populated and Area 14 to have 5 shaded
	grid[40] = grid[41] = true;

	// Area 3 - Remove 3
	grid[9] = grid[15] = grid[16] = grid[21] = grid[22] = grid[23] = grid[29] = grid[36] = true;

	static std::vector<int> v3rem3 = { 9, 15, 16, 21, 22, 23, 29, 36 };

	// Area 1 - Cell 5 must be white from area 11
	grid[2] = grid[3] = grid[4] = grid[9] = grid[11] = true;

	// Area 4 - Already complete by Areas 1 and 21

	// Area 5 - Remove 1 - Cell 3 must be shaded from area 21
	grid[4] = grid[10] = grid[11] = grid[12] = grid[17] = true;

	static std::vector<int> v5rem1 = { 4, 10, 11, 12, 17 };

	// Area 7 - Remove 2 - Cell 41 must be shaded for centre cell to be connected
	grid[26] = grid[32] = grid[33] = grid[34] = grid[38] = grid[39] = true;

	static std::vector<int> v7rem2 = { 26, 32, 33, 34, 38, 39 };

	// Area 6 - Remove 0 or 1 at most depending on Area 7 (cell 38)
	grid[18] = grid[19] = grid[20] = grid[25] = grid[27] = true;

	static std::vector<int> v6rem1poss = { 18, 19, 20, 25, 27 };

	for (int i3_1 = 0; i3_1 < (int)v3rem3.size() - 2; ++i3_1) {
		for (int i3_2 = i3_1 + 1; i3_2 < (int)v3rem3.size() - 1; ++i3_2) {
			for (int i3_3 = i3_2 + 1; i3_3 < (int)v3rem3.size(); ++i3_3) {
				grid[v3rem3[i3_1]] = false;
				grid[v3rem3[i3_2]] = false;
				grid[v3rem3[i3_3]] = false;

				for (const int& i5 : v5rem1) {
					grid[i5] = false;

					// Area 16 is covered by Areas 3, 4, 5
					// Area 9 is covered by Areas 3, 4, 5
					if ((grid[1] + grid[2] + grid[8] + grid[9] + grid[16] + grid[17] == shaded_per_area) &&
						(grid[0] + grid[1] + grid[8] + grid[10] + grid[14] + grid[15] + grid[16] + grid[17] == shaded_per_area)) {

						for (int i7_1 = 0; i7_1 < (int)v7rem2.size() - 1; ++i7_1) {
							for (int i7_2 = i7_1 + 1; i7_2 < (int)v7rem2.size(); ++i7_2) {
								grid[v7rem2[i7_1]] = false;
								grid[v7rem2[i7_2]] = false;

								for (int i6_1poss = 0; i6_1poss < (grid[38] ? (int)v6rem1poss.size() : 0); ++i6_1poss) {
									if (grid[38]) { grid[v6rem1poss[i6_1poss]] = false; }

									// Area 12 is covered by Areas 6 and 7
									// Area 8 is covered by Areas 6 and 7 (cell 40 is a must from Area 14)
									if ((grid[18] + grid[20] + grid[25] + grid[26] + grid[27] + grid[38] == shaded_per_area) &&
										(grid[18] + grid[25] + grid[26] + grid[33] + grid[34] + grid[40] + grid[41] == shaded_per_area)) {

										// Area 15 is covered by Areas 5, 6
										if (grid[3] + grid[4] + grid[10] + grid[11] + grid[12] + grid[19] == shaded_per_area) {

											// Area 17 is covered by Areas 3, 4 and 2 (cell 28)
											int area17countexcell28 = grid[0] + grid[7] + grid[14] + grid[15] + grid[21] + grid[29];
											if (area17countexcell28 >= (shaded_per_area - 1) && area17countexcell28 <= shaded_per_area) {
												if (area17countexcell28 == (shaded_per_area - 1)) {
													grid[28] = true;
												}

												// Area 20 is covered by Areas 3, 4 and 2 (cell 28, 35) - Cell 35 is white by Area 11
												if (grid[2] + grid[9] + grid[8] + grid[15] + grid[21] + grid[22] + grid[28] + grid[29] == shaded_per_area) {

													// Area 19 is covered by Areas 3, 5, 6, 7 and 2 (cell 24)
													int area19countexcell24 = grid[12] + grid[16] + grid[19] + grid[23] + grid[25] + grid[26];
													if (area19countexcell24 >= (shaded_per_area - 1) && area19countexcell24 <= shaded_per_area) {
														if (area19countexcell24 == (shaded_per_area - 1)) {
															grid[24] = true;
														}

														// Area 10 is covered by Areas 2, 3, 4
														if (grid[7] + grid[21] + grid[22] + grid[23] + grid[24] + grid[28] + grid[29] + grid[35] == shaded_per_area) {

															// Area 2
															if (grid[6] + grid[12] + grid[13] + grid[19] + grid[20] + grid[27] + grid[28] + grid[35]
																+ grid[24] + grid[30] + grid[31] + grid[37] == shaded_per_area) {

																// Area 13
																if (grid[22] + grid[23] + grid[24] + grid[30] + grid[31] + grid[32] + grid[38] + grid[27] == shaded_per_area) {

																	// Area 18 - Cells 5, 6, 13 is white by Area 11
																	if (grid[4] + grid[10] + grid[11] + grid[17] + grid[18] + grid[20] == shaded_per_area) {

																		// Area 11
																		if ((2 * grid[32]) + (2 * grid[33]) + (2 * grid[34]) == (shaded_per_area - 1)) { // Reserve 1 for centre cell

																			std::vector<bool> full_grid(areas_side_len * areas_side_len, false);
																			populate_full_grid(full_grid, areas_side_len, grid);

																			if (is_shaded_connected(full_grid) &&
																				is_shaded_count_equal_between_areas(m_areas, m_areas_counts, full_grid, shaded_per_area)) {

																				std::cout << "Solution:" << std::endl;
																				print_grid(m_areas, full_grid);

																				int product = calculate_unshaded_regions_product(full_grid);

																				if (product < min_product) {
																					min_product = product;
																				}
																			}
																		}
																	}
																}
															}
														}
														grid[24] = false;
													}
												}
												grid[28] = false;
											}
										}
									}

									if (grid[38]) { grid[v6rem1poss[i6_1poss]] = true; }
								}

								grid[v7rem2[i7_1]] = true;
								grid[v7rem2[i7_2]] = true;
							}
						}

					}
					grid[i5] = true;
				}

				grid[v3rem3[i3_1]] = true;
				grid[v3rem3[i3_2]] = true;
				grid[v3rem3[i3_3]] = true;
			}
		}
	}
}
