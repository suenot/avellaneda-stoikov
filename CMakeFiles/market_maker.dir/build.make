# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/suenot/work_dev_3/avellaneda-stoikov

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/suenot/work_dev_3/avellaneda-stoikov

# Include any dependencies generated for this target.
include CMakeFiles/market_maker.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/market_maker.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/market_maker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/market_maker.dir/flags.make

CMakeFiles/market_maker.dir/codegen:
.PHONY : CMakeFiles/market_maker.dir/codegen

CMakeFiles/market_maker.dir/src/main.cpp.o: CMakeFiles/market_maker.dir/flags.make
CMakeFiles/market_maker.dir/src/main.cpp.o: src/main.cpp
CMakeFiles/market_maker.dir/src/main.cpp.o: CMakeFiles/market_maker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/suenot/work_dev_3/avellaneda-stoikov/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/market_maker.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/market_maker.dir/src/main.cpp.o -MF CMakeFiles/market_maker.dir/src/main.cpp.o.d -o CMakeFiles/market_maker.dir/src/main.cpp.o -c /Users/suenot/work_dev_3/avellaneda-stoikov/src/main.cpp

CMakeFiles/market_maker.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/market_maker.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/suenot/work_dev_3/avellaneda-stoikov/src/main.cpp > CMakeFiles/market_maker.dir/src/main.cpp.i

CMakeFiles/market_maker.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/market_maker.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/suenot/work_dev_3/avellaneda-stoikov/src/main.cpp -o CMakeFiles/market_maker.dir/src/main.cpp.s

CMakeFiles/market_maker.dir/src/market_maker.cpp.o: CMakeFiles/market_maker.dir/flags.make
CMakeFiles/market_maker.dir/src/market_maker.cpp.o: src/market_maker.cpp
CMakeFiles/market_maker.dir/src/market_maker.cpp.o: CMakeFiles/market_maker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/suenot/work_dev_3/avellaneda-stoikov/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/market_maker.dir/src/market_maker.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/market_maker.dir/src/market_maker.cpp.o -MF CMakeFiles/market_maker.dir/src/market_maker.cpp.o.d -o CMakeFiles/market_maker.dir/src/market_maker.cpp.o -c /Users/suenot/work_dev_3/avellaneda-stoikov/src/market_maker.cpp

CMakeFiles/market_maker.dir/src/market_maker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/market_maker.dir/src/market_maker.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/suenot/work_dev_3/avellaneda-stoikov/src/market_maker.cpp > CMakeFiles/market_maker.dir/src/market_maker.cpp.i

CMakeFiles/market_maker.dir/src/market_maker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/market_maker.dir/src/market_maker.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/suenot/work_dev_3/avellaneda-stoikov/src/market_maker.cpp -o CMakeFiles/market_maker.dir/src/market_maker.cpp.s

CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o: CMakeFiles/market_maker.dir/flags.make
CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o: src/inventory_manager.cpp
CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o: CMakeFiles/market_maker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/suenot/work_dev_3/avellaneda-stoikov/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o -MF CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o.d -o CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o -c /Users/suenot/work_dev_3/avellaneda-stoikov/src/inventory_manager.cpp

CMakeFiles/market_maker.dir/src/inventory_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/market_maker.dir/src/inventory_manager.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/suenot/work_dev_3/avellaneda-stoikov/src/inventory_manager.cpp > CMakeFiles/market_maker.dir/src/inventory_manager.cpp.i

CMakeFiles/market_maker.dir/src/inventory_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/market_maker.dir/src/inventory_manager.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/suenot/work_dev_3/avellaneda-stoikov/src/inventory_manager.cpp -o CMakeFiles/market_maker.dir/src/inventory_manager.cpp.s

# Object files for target market_maker
market_maker_OBJECTS = \
"CMakeFiles/market_maker.dir/src/main.cpp.o" \
"CMakeFiles/market_maker.dir/src/market_maker.cpp.o" \
"CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o"

# External object files for target market_maker
market_maker_EXTERNAL_OBJECTS =

market_maker: CMakeFiles/market_maker.dir/src/main.cpp.o
market_maker: CMakeFiles/market_maker.dir/src/market_maker.cpp.o
market_maker: CMakeFiles/market_maker.dir/src/inventory_manager.cpp.o
market_maker: CMakeFiles/market_maker.dir/build.make
market_maker: /opt/homebrew/lib/libboost_system.dylib
market_maker: CMakeFiles/market_maker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/suenot/work_dev_3/avellaneda-stoikov/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable market_maker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/market_maker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/market_maker.dir/build: market_maker
.PHONY : CMakeFiles/market_maker.dir/build

CMakeFiles/market_maker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/market_maker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/market_maker.dir/clean

CMakeFiles/market_maker.dir/depend:
	cd /Users/suenot/work_dev_3/avellaneda-stoikov && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/suenot/work_dev_3/avellaneda-stoikov /Users/suenot/work_dev_3/avellaneda-stoikov /Users/suenot/work_dev_3/avellaneda-stoikov /Users/suenot/work_dev_3/avellaneda-stoikov /Users/suenot/work_dev_3/avellaneda-stoikov/CMakeFiles/market_maker.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/market_maker.dir/depend

