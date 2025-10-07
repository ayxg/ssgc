# SSGMono : CMake Monorepo Module
A set of CMake scripts and configs files for monorepos to manage internal 
subprojects as a set of interdependent packages. 

## Use Case & Benefits:

The main use case for this module is for subprojects to be consumed through the same interface as 
externally installed projects(`find_package`). Each subproject generates a package configuration
file mimicing CMake's `package-config` files. The function `ssg_export_subproject` handles all 
necessary files for the project to be found using `find_package`. For example, given a monorepo
containing a root `CMakeLists.txt`. 
Inside the subproject's `project/foo_project/CMakeLists.txt`:
``` cmake
project(foo_project)

# sub project's targets/vars etc... 
...

# export subproject
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	MODULE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
	TARGETS FooExecutable BarLibrary
	REDIRECT_FIND_PACKAGE
)
```
Then in the monorepo's root `CMakeLists.txt` you would declare the subproject and use `find_package` to
include it in the build (see install section on how to consume mono as a subproject too):
``` cmake
find_package(ssgmono_box REQUIRED CONFIG)
include(SsgProjectPackageUtils)

ssg_declare_subprojects(
	NAMES 
		foo_project
	SOURCE_DIRS
		"${CMAKE_CURRENT_SOURCE_DIR}/projects/foo_project"
)

# Aquire external subprojects.
include(SsgGitPkg)
ssg_gitpkg_imgui_sfml()
ssg_gitpkg_nlohmanjson()

# Find dependencies
find_package(foo_project REQUIRED CONFIG)
```
This strategy makes the project part of the build and consumable by other subprojects. Another advantage
is that dependencies between subprojects are tracked by CMake's `find_package` DAG. 
Only subprojects which are dependencies of the package requested by the root `CMakeLists.txt` are built. 
They are also replaceable by external packages to easily test different versions or experimental features,
by virtue of using the `find_package` interface internally. Furthermore, the strategy maintains the build 
directory of the monorepo clean. All subprojects are built into the CMake folder `CMakeFiles/_deps` and 
organized into sub-build and build folders by CMake's `find_package` and `FetchContent` features.

## Modules
- `SsgCMakeUtils`: `ssg_assert_arguments`,`ssg_check_unparsed_arguments` 
- `SsgEmbedFiles`: Helper for generating C++ headers/sources that embed binary files into a target.
- `SsgGitPkg` : Basic package provider, currently only has external packages used by the `ssgc` monorepo.
- `SsgProjectBase` : **Deprecated.** Used to provide base variables for all `ssgc` monorepo projects. It
  is not reccomended to add global project variables. 
- `SsgProjectPackageUtils`: Main module. Create and consume subproject package files.

## Installation
### Option 1: Install and use as an external dependency
Clone the repo. Configure and build the cmake project. Inside the binary directory use:
``` bash
cmake --install .
```
In your projects `CMakeLists.txt`:
``` cmake
find_package(ssgmono_box REQUIRED CONFIG)
```

### Option 2: Add as a subproject.
Clone the repo into your monorepo, or copy the files.(You may want to delete the .git folder or do use a
git bare clone). Given you placed the files in `projects/ssgmono_box` relative to your root CMake source
dir. In your monorepo's `CMakeLists.txt`:
``` cmake
include(FetchContent)

# ssgmono_box - ssg monorepo config scripts
# Manually add AND find 'ssgmono_box'.
# Afterwards use the 'SsgProjectPackageUtils' module function from ssgmono :
# to add subprojects in this project.
#
# Other top-level projects should use 'find_package(ssgmono_box)' instead,
# when using the 'ssgmono_box' project a dependency.
FetchContent_Declare(ssgmono_box 
  SOURCE_DIR "projects/ssgmono_box" 
  OVERRIDE_FIND_PACKAGE
)
find_package(ssgmono_box REQUIRED CONFIG)
```