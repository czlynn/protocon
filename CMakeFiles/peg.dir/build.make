# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zhilinchen/Documents/GitHub/protocon

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zhilinchen/Documents/GitHub/protocon

# Utility rule file for peg.

# Include the progress variables for this target.
include CMakeFiles/peg.dir/progress.make

CMakeFiles/peg: CMakeFiles/peg-complete


CMakeFiles/peg-complete: external/peg/src/peg-stamp/peg-install
CMakeFiles/peg-complete: external/peg/src/peg-stamp/peg-mkdir
CMakeFiles/peg-complete: external/peg/src/peg-stamp/peg-download
CMakeFiles/peg-complete: external/peg/src/peg-stamp/peg-update
CMakeFiles/peg-complete: external/peg/src/peg-stamp/peg-patch
CMakeFiles/peg-complete: external/peg/src/peg-stamp/peg-configure
CMakeFiles/peg-complete: external/peg/src/peg-stamp/peg-build
CMakeFiles/peg-complete: external/peg/src/peg-stamp/peg-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'peg'"
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles/peg-complete
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src/peg-stamp/peg-done

external/peg/src/peg-stamp/peg-install: external/peg/src/peg-stamp/peg-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'peg'"
	cd /Users/zhilinchen/Documents/GitHub/protocon/dep/peg && echo "No install step."
	cd /Users/zhilinchen/Documents/GitHub/protocon/dep/peg && /Applications/CMake.app/Contents/bin/cmake -E touch /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src/peg-stamp/peg-install

external/peg/src/peg-stamp/peg-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'peg'"
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/zhilinchen/Documents/GitHub/protocon/dep/peg
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/zhilinchen/Documents/GitHub/protocon/dep/peg
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/zhilinchen/Documents/GitHub/protocon/external/peg
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/zhilinchen/Documents/GitHub/protocon/external/peg/tmp
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src/peg-stamp
	/Applications/CMake.app/Contents/bin/cmake -E make_directory /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src/peg-stamp/peg-mkdir

external/peg/src/peg-stamp/peg-download: external/peg/src/peg-stamp/peg-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'peg'"
	/Applications/CMake.app/Contents/bin/cmake -E echo_append
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src/peg-stamp/peg-download

external/peg/src/peg-stamp/peg-update: external/peg/src/peg-stamp/peg-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Performing update step for 'peg'"
	cd /Users/zhilinchen/Documents/GitHub/protocon/dep/peg && echo "No update."

external/peg/src/peg-stamp/peg-patch: external/peg/src/peg-stamp/peg-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'peg'"
	/Applications/CMake.app/Contents/bin/cmake -E echo_append
	/Applications/CMake.app/Contents/bin/cmake -E touch /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src/peg-stamp/peg-patch

external/peg/src/peg-stamp/peg-configure: external/peg/tmp/peg-cfgcmd.txt
external/peg/src/peg-stamp/peg-configure: external/peg/src/peg-stamp/peg-update
external/peg/src/peg-stamp/peg-configure: external/peg/src/peg-stamp/peg-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'peg'"
	cd /Users/zhilinchen/Documents/GitHub/protocon/dep/peg && echo "No configure step."
	cd /Users/zhilinchen/Documents/GitHub/protocon/dep/peg && /Applications/CMake.app/Contents/bin/cmake -E touch /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src/peg-stamp/peg-configure

external/peg/src/peg-stamp/peg-build: external/peg/src/peg-stamp/peg-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'peg'"
	cd /Users/zhilinchen/Documents/GitHub/protocon/dep/peg && $(MAKE)
	cd /Users/zhilinchen/Documents/GitHub/protocon/dep/peg && /Applications/CMake.app/Contents/bin/cmake -E touch /Users/zhilinchen/Documents/GitHub/protocon/external/peg/src/peg-stamp/peg-build

peg: CMakeFiles/peg
peg: CMakeFiles/peg-complete
peg: external/peg/src/peg-stamp/peg-install
peg: external/peg/src/peg-stamp/peg-mkdir
peg: external/peg/src/peg-stamp/peg-download
peg: external/peg/src/peg-stamp/peg-update
peg: external/peg/src/peg-stamp/peg-patch
peg: external/peg/src/peg-stamp/peg-configure
peg: external/peg/src/peg-stamp/peg-build
peg: CMakeFiles/peg.dir/build.make

.PHONY : peg

# Rule to build all files generated by this target.
CMakeFiles/peg.dir/build: peg

.PHONY : CMakeFiles/peg.dir/build

CMakeFiles/peg.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/peg.dir/cmake_clean.cmake
.PHONY : CMakeFiles/peg.dir/clean

CMakeFiles/peg.dir/depend:
	cd /Users/zhilinchen/Documents/GitHub/protocon && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhilinchen/Documents/GitHub/protocon /Users/zhilinchen/Documents/GitHub/protocon /Users/zhilinchen/Documents/GitHub/protocon /Users/zhilinchen/Documents/GitHub/protocon /Users/zhilinchen/Documents/GitHub/protocon/CMakeFiles/peg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/peg.dir/depend

