## conformance_test.py

### Run clang-tidy on NumCSE codes and filter out warnings from selected third-party includes. 

Comitted a first version of a custom clang-tidy script for the NumCSE repository. It is too heavy to run on a large portion of the repository. It also relies heavily on cmake's 'Generate' output 'compile_commands.json', mostly because clang-tidy needs those to understand includes. The script uses a custom suppression mechanism for warnings generated by clang-tidy about matplotlibcpp and Eigen, which cannot (at least I was not able to figure it out) be ignored with the usual techniques used in clang-tidy. This is probably owed to the way we include those libraries. 

Usage is documented in the file itself, however here is minimal example which should work without errors:

 1. Change directory to your build folder (wherever cmake outputs binaries, most likely directly in your repository)
 2. Run `python ~/repositories/NumCSE/Clang-tidy/conformance_test.py -d ~/repositories/NumCSE/LectureCodes/EigenTutorial/`.

(Possibly, you have to modify the paths so they point to the NumCSE repository).

This will run only clang-tidy on all the Eigen tutorial lecture codes and apply appropriate filters. 

#### Usage 
`python conformance_test.py [-f <file> |-d <directory>] [-p <build_directory>] [--no_filter] [--format]`
- `-f`: process only a single file `<file>`
- `-d`: process a whole directory `<directory>`
-	neither `-f` nor `-d`: process the working directory
-	`-p`: specify the build directory `<build_directory>`, **default**: working directory
- `--no_filter`: do not filter out irrelevant warnings
- `--format`: additionally run clang-format on all processed files