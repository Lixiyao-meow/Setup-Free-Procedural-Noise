# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xiyao/Documents/INF584/02_MyRenderer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiyao/Documents/INF584/02_MyRenderer/build

# Utility rule file for glad-generate-files.

# Include the progress variables for this target.
include External/glad/CMakeFiles/glad-generate-files.dir/progress.make

External/glad/CMakeFiles/glad-generate-files: External/glad/src/glad.c
External/glad/CMakeFiles/glad-generate-files: External/glad/include/glad/glad.h


External/glad/src/glad.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiyao/Documents/INF584/02_MyRenderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating GLAD"
	cd /home/xiyao/Documents/INF584/02_MyRenderer/External/glad && /usr/bin/python3.8 -m glad --profile=core --out-path=/home/xiyao/Documents/INF584/02_MyRenderer/build/External/glad --api=gl=4.5 --generator=c --extensions= --spec=gl

External/glad/include/glad/glad.h: External/glad/src/glad.c
	@$(CMAKE_COMMAND) -E touch_nocreate External/glad/include/glad/glad.h

glad-generate-files: External/glad/CMakeFiles/glad-generate-files
glad-generate-files: External/glad/src/glad.c
glad-generate-files: External/glad/include/glad/glad.h
glad-generate-files: External/glad/CMakeFiles/glad-generate-files.dir/build.make

.PHONY : glad-generate-files

# Rule to build all files generated by this target.
External/glad/CMakeFiles/glad-generate-files.dir/build: glad-generate-files

.PHONY : External/glad/CMakeFiles/glad-generate-files.dir/build

External/glad/CMakeFiles/glad-generate-files.dir/clean:
	cd /home/xiyao/Documents/INF584/02_MyRenderer/build/External/glad && $(CMAKE_COMMAND) -P CMakeFiles/glad-generate-files.dir/cmake_clean.cmake
.PHONY : External/glad/CMakeFiles/glad-generate-files.dir/clean

External/glad/CMakeFiles/glad-generate-files.dir/depend:
	cd /home/xiyao/Documents/INF584/02_MyRenderer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiyao/Documents/INF584/02_MyRenderer /home/xiyao/Documents/INF584/02_MyRenderer/External/glad /home/xiyao/Documents/INF584/02_MyRenderer/build /home/xiyao/Documents/INF584/02_MyRenderer/build/External/glad /home/xiyao/Documents/INF584/02_MyRenderer/build/External/glad/CMakeFiles/glad-generate-files.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : External/glad/CMakeFiles/glad-generate-files.dir/depend

