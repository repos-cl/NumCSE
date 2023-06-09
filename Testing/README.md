# Instructions to update CodeExpert tests.

This assumes that you have installed the necessary libraries
as documented in `NumCSE/README.md`.

In the following, assume EX is the path to the exercise
project you want to modify. For example: 
	`NumCSE/Assignments/Codes/ODE/CrossProd/`

## Main instructions
1. In `CMakeLists.txt` in `EX/CodeExpert`, add the 
line `add_tests_numcse(tests template/tests.cpp)` to the bottom.
2. Copy the `tests.cpp` from `NumCSE/Testing` to `EX/CodeExpert/template`
3. If the exercise file contains multiline function names, change 
them to singleline.
	(a) `template <class Function>` and other templates should be on a
	separate line above.
4. Modify it according to the next section
5. Copy `tests.cpp` from `EX/CodeExpert/template` to
`EX/CodeExpert/solution`
6. Remove the following from `template`, `solution` folders:
`conf.yml`, `zzzzz_test_runner.cpp`, `tests.csv`, `scripts` (a folder)
	(a) You can do this by running this from `EX/CodeExpert`: 
	`rm -r template/scripts template/zzzzz_test_runner.cpp template/conf.yml template/tests.csv solution/scripts solution/zzzzz_test_runner.cpp solution/conf.yml solution/tests.csv`
7. Add `#pragma once` to exercise/supporting files if needed.
Look for "redefined" errors.
8. Compile and test (see below)

## Modifying tests.cpp: 
1. Remove the top comment
2. Follow comments to add test cases. Use the test case names
from `tests.csv` in the doctest descriptions. 
	(a) The test cases are usually defined in `EX/template/main.cpp`, 
	sometimes in the `tests.csv` file. You should copy the behaviour.
	Sometimes you will need to copy data. In this case, 
	copy it into the `TestData` struct. See example below.
	Refer to parts of it by `data.VAR_NAME`.
	
	(b) Sometimes you will need to include other libraries. 
	Look at what `main.cpp` and `EX_NAME.hpp` include. 
	For example, CrossProd needed `#include <vector>`. 
	
	(c) If a test case defined in `main.cpp` or `tests.csv` isn't
	testable or doesn't make sense to test, then 
	skip it according to the template function in `tests.cpp`.
	Make note of this though so we know about it.
3. Follow the guidlines in `tests.cpp` for more information.
	
TestData example: 
``` cpp
	struct TestData {
		TestData() {
			T = 1.;
			N = 1;
			y0 << 0.1, 0.2, 0.4;
			f = [] (const Vector3d &y) -> Vector3d {
				Vector3d fy;
				fy << y(0)*y(1), y(1)*y(2), y(2)-y(0);
				return fy;
			};
			Jf = [] (const Vector3d &y) {
				Matrix3d J;
				J << y(1),y(0),0,0,y(2),y(1),-1,0,1;
				return J;
			};
		}
		
		double T;
		int N;
		Vector3d y0;
		std::function<Vector3d(const Vector3d&)> f;
		std::function<Matrix3d(const Vector3d&)> Jf;
	};
```
	
## Compiling, testing: 
(first 2 steps skippable if you've already done them)
1. `cd` to `NumCSE/`
2. run: `mkdir build && cd build && cmake ..`
	(a) This creates the CMake hierarchy
3. `cd` to your exercise but inside `NumCSE/build/`
	(a) Instead of `NumCSE/Assignments/...` it will be
	`NumCSE/build/Assignments/...`
4. In the CodeExpert folder of your assignment, run `make`
	(a) This should be repeated every time you change something
	in the assignment folder.
5. To run the executables you just compiled, `cd` to the exercise
but in `NumCSE/build/bin/Assignments/...`. Then run whatever
binary you want. There should be solution, template, and tests.
For example: run `./tests`.
	(a) It's a good idea to change the student file
	(in `EX/CodeExpert/template/EX_NAME.hpp`) to be the solution,
	then run `make` again to ensure the tests pass on the correct
	answer.

