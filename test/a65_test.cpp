/**
 * A65
 * Copyright (C) 2018 David Jolly
 *
 * A65 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A65 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "./a65_test_type.h"

bool
is_file_matching(
	__in const std::string &left,
	__in const std::string &right
	)
{
	bool result = true;

	try {
		std::vector<uint8_t> left_data, right_data;

		a65_utility::read_file(left, left_data);
		a65_utility::read_file(right, right_data);

		result = (left_data.size() == right_data.size());
		if(result) {

			for(size_t offset = 0; offset < left_data.size(); ++offset) {

				result = (left_data.at(offset) == right_data.at(offset));
				if(!result) {
					break;
				}
			}
		}
	} catch(...) {
		result = false;
	}

	return result;
}

bool
run_functional_test(
	__in int test
	)
{
	bool result = true;

	std::stringstream path;

	path << A65_TEST_DIRECTORY(test) << A65_TEST_FUNCTIONAL_SOURCE;

	result = (a65_build_object(path.str().c_str(), A65_TEST_OUTPUT, 0, 0) == EXIT_SUCCESS);
	if(result) {
		std::string input_path;
		std::stringstream old_path, new_path;

		old_path << A65_TEST_DIRECTORY(test) << A65_TEST_FUNCTIONAL_OBJECT;
		new_path << A65_TEST_OUTPUT << A65_TEST_FUNCTIONAL_OBJECT_OUTPUT;

		result = is_file_matching(old_path.str(), new_path.str());
		if(result) {
			path.clear();
			path.str(std::string());
			path << A65_TEST_OUTPUT << A65_TEST_FUNCTIONAL_OBJECT_OUTPUT;
			input_path = path.str();
			const char *input = &input_path[0];

			result = (a65_compile(1, &input, A65_TEST_OUTPUT, A65_TEST_FUNCTIONAL_BINARY_NAME, 1, 0) == EXIT_SUCCESS);
			if(result) {
				old_path.clear();
				old_path.str(std::string());
				old_path << A65_TEST_DIRECTORY(test) << A65_TEST_FUNCTIONAL_BINARY;

				new_path.clear();
				new_path.str(std::string());
				new_path << A65_TEST_OUTPUT << A65_TEST_FUNCTIONAL_BINARY;

				result = is_file_matching(old_path.str(), new_path.str());
			}
		}
	}

	return result;
}

int
run_functional_tests(void)
{
	int result = EXIT_SUCCESS;

	for(int test = A65_TEST_FUNCTIONAL_MIN; test <= A65_TEST_FUNCTIONAL_MAX; ++test) {
		std::cout << A65_COLUMN_WIDTH(A65_TEST_COLUMN_WIDTH) << A65_TEST_STRING(test) << "[";

		result = (run_functional_test(test) ? EXIT_SUCCESS : EXIT_FAILURE);

		std::cout << ((result == EXIT_SUCCESS) ? "PASS" : "FAIL") << "]" << std::endl;
	}

	return result;
}

int
main(void)
{
	int result = EXIT_SUCCESS;

	result = run_functional_tests();

	return result;
}
