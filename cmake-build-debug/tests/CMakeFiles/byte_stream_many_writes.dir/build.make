# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /home/llt/Programs/CLion-2020.2/clion-2020.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/llt/Programs/CLion-2020.2/clion-2020.2/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/llt/github/TCP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/llt/github/TCP/cmake-build-debug

# Include any dependencies generated for this target.
include tests/CMakeFiles/byte_stream_many_writes.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/byte_stream_many_writes.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/byte_stream_many_writes.dir/flags.make

tests/CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.o: tests/CMakeFiles/byte_stream_many_writes.dir/flags.make
tests/CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.o: ../tests/byte_stream_many_writes.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/llt/github/TCP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.o"
	cd /home/llt/github/TCP/cmake-build-debug/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.o -c /home/llt/github/TCP/tests/byte_stream_many_writes.cc

tests/CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.i"
	cd /home/llt/github/TCP/cmake-build-debug/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/llt/github/TCP/tests/byte_stream_many_writes.cc > CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.i

tests/CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.s"
	cd /home/llt/github/TCP/cmake-build-debug/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/llt/github/TCP/tests/byte_stream_many_writes.cc -o CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.s

# Object files for target byte_stream_many_writes
byte_stream_many_writes_OBJECTS = \
"CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.o"

# External object files for target byte_stream_many_writes
byte_stream_many_writes_EXTERNAL_OBJECTS =

tests/byte_stream_many_writes: tests/CMakeFiles/byte_stream_many_writes.dir/byte_stream_many_writes.cc.o
tests/byte_stream_many_writes: tests/CMakeFiles/byte_stream_many_writes.dir/build.make
tests/byte_stream_many_writes: tests/libspongechecks.a
tests/byte_stream_many_writes: libsponge/libsponge.a
tests/byte_stream_many_writes: tests/CMakeFiles/byte_stream_many_writes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/llt/github/TCP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable byte_stream_many_writes"
	cd /home/llt/github/TCP/cmake-build-debug/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/byte_stream_many_writes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/byte_stream_many_writes.dir/build: tests/byte_stream_many_writes

.PHONY : tests/CMakeFiles/byte_stream_many_writes.dir/build

tests/CMakeFiles/byte_stream_many_writes.dir/clean:
	cd /home/llt/github/TCP/cmake-build-debug/tests && $(CMAKE_COMMAND) -P CMakeFiles/byte_stream_many_writes.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/byte_stream_many_writes.dir/clean

tests/CMakeFiles/byte_stream_many_writes.dir/depend:
	cd /home/llt/github/TCP/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/llt/github/TCP /home/llt/github/TCP/tests /home/llt/github/TCP/cmake-build-debug /home/llt/github/TCP/cmake-build-debug/tests /home/llt/github/TCP/cmake-build-debug/tests/CMakeFiles/byte_stream_many_writes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/byte_stream_many_writes.dir/depend

