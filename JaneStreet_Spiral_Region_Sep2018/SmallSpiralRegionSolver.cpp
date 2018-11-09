#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>

// 
class SmallSpiralRegionSolver {

	const std::vector<int> m_areas;

	std::map<int, int> m_areas_counts;

	int min_product;

public:

	SmallSpiralRegionSolver(const std::vector<int> &areas) : m_areas(areas), min_product(INT_MAX) {

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

			if (shaded_count % 4 == 0) {

				int half_size = areas_side_len / 2;
				std::vector<bool> grid((half_size * 2) * (half_size * 2), false);

				recursive_shade_cells(half_size, (half_size * half_size), grid, (shaded_count / 4), shaded_per_area, areas_side_len);
			}
			else if ((shaded_count - 1) % 4 == 0) {

				std::cout << "NOT IMPLEMENTED" << std::endl;

			}
		}

		std::cout << "Min Product: " << min_product << std::endl;
	}

private:

	//
	int count_shaded(const std::vector<bool> &grid) const {
		int count = 0;
		for (std::vector<bool>::const_iterator it = grid.begin(); it != grid.end(); ++it) {
			if (*it == true) {
				++count;
			}
		}
		return count;
	}

	//
	/*bool is_shaded_count_equal_between_areas(std::vector<bool> grid, const int &shaded_per_area) const {
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
	}*/

	//
	int switch_shading_within_connected_region(std::vector<bool> &grid, const int &index, const bool &target_state) const {
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

	// 
	// NB. Param copy passed by value
	bool is_shaded_connected(std::vector<bool> copy) const {
		// Find an index which is shaded as starting point
		for (size_t i = 0; i < copy.size(); ++i) {
			if (copy[i]) {
				switch_shading_within_connected_region(copy, (int)i, false);
				break;
			}
		}

		return count_shaded(copy) == 0;
	}

	//
	void print_grid(const std::vector<bool> &grid) const {
		const int size = (int)sqrt((double)grid.size());

		for (int i = 0; i < (int)grid.size(); ++i) {
			std::cout << std::setw(2) << std::setfill('0') << (grid[i] ? m_areas[i] : 0) << " ";
			if ((i + 1) % size == 0) {
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
	}

	// 
	// NB. Param copy passed by value
	int calculate_unshaded_regions_product(std::vector<bool> copy) {
		int product = 1;

		while (count_shaded(copy) != (int)copy.size()) {
			// Find an index which is not shaded as starting point
			for (size_t i = 0; i < copy.size(); ++i) {
				if (!copy[i]) {
					int count = switch_shading_within_connected_region(copy, (int)i, true);
					std::cout << (i == 0 ? "" : " * ") << count;
					product *= count;
					break;
				}
			}
		}
		std::cout << " = " << product << std::endl;
		return product;
	}

	//
	void recursive_shade_cells(const int &half_size, const int &recursion_index, std::vector<bool> &grid,
		const int &remaining_to_shade, const int & shaded_per_area, const int &areas_side_len) {

		if (remaining_to_shade == 0) {
			/*static long long int trials = 0;
			++trials;
			if (trials % 1000000 == 0) {
				std::cout << "Processed: " << trials << std::endl;
				print_grid(grid);
			}*/

			if (!is_shaded_connected(grid)) { return; }

			int min_x = INT_MAX, min_y = INT_MAX;
			int max_x = -1, max_y = -1;

			for (int i = 0; i < (int)grid.size(); ++i) {
				if (grid[i]) {
					int x = i % (half_size * 2);
					int y = i / (half_size * 2);

					if (x < min_x) { min_x = x; }
					if (x > max_x) { max_x = x; }
					if (y < min_y) { min_y = y; }
					if (y > max_y) { max_y = y; }
				}
			}

			for (int x_offset = -min_x; x_offset <= (areas_side_len - max_x - 1); ++x_offset) {
				for (int y_offset = -min_y; y_offset <= (areas_side_len - max_y - 1); ++y_offset) {
					std::vector<bool> full_grid(areas_side_len * areas_side_len, false);

					std::map<int, int> m;
					bool areas_shaded_valid = true;

					for (int i = 0; i < (int)grid.size(); ++i) {
						if (grid[i]) {

							int x = i % (2 * half_size);
							int y = i / (2 * half_size);

							int j = (x + x_offset) + ((y + y_offset) * areas_side_len);
							full_grid[j] = true;

							if (++m[m_areas[j]] > shaded_per_area) {
								areas_shaded_valid = false;
								break;
							}
						}
					}

					if (areas_shaded_valid) {

						std::cout << "Solution:" << std::endl;
						print_grid(full_grid);

						int product = calculate_unshaded_regions_product(full_grid);

						if (product < min_product) {
							min_product = product;
						}
					}
				}
			}
			return;
		}

		if (recursion_index == 0) {
			return;
		}

		//
		for (int i = (recursion_index > remaining_to_shade ? 0 : 1); i <= 1; ++i) {
			int x, y, x_rot1, y_rot1, x_rot2, y_rot2, x_rot3, y_rot3, xy_cen = (half_size - 1);
			x = y = x_rot1 = y_rot1 = x_rot2 = y_rot2 = x_rot3 = y_rot3 = 0;

			if (i == 1) {
				x = ((recursion_index - 1) % half_size) + half_size;
				y = ((recursion_index - 1) / half_size) + half_size;
				grid[x + (y * half_size * 2)] = true;

				x_rot1 = xy_cen - (y - xy_cen - 1);
				y_rot1 = x;
				grid[x_rot1 + (y_rot1 * half_size * 2)] = true;

				x_rot2 = xy_cen - (y_rot1 - xy_cen - 1);
				y_rot2 = x_rot1;
				grid[x_rot2 + (y_rot2 * half_size * 2)] = true;

				x_rot3 = xy_cen - (y_rot2 - xy_cen - 1);
				y_rot3 = x_rot2;
				grid[x_rot3 + (y_rot3 * half_size * 2)] = true;
			}

			recursive_shade_cells(half_size, (recursion_index - 1), grid, (remaining_to_shade - i), shaded_per_area, areas_side_len);

			if (i == 1) {
				grid[x + (y * half_size * 2)] = false;
				grid[x_rot1 + (y_rot1 * half_size * 2)] = false;
				grid[x_rot2 + (y_rot2 * half_size * 2)] = false;
				grid[x_rot3 + (y_rot3 * half_size * 2)] = false;
			}
		}
	}

};
