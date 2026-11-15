cmake_minimum_required(VERSION 3.10)
project(SmartAutocompleteSearchEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Aszad's files + the team's files.
# Add Dewang's, Ravinder's and Piyush's .cpp files, and the AppServices.cpp
# that connects everything, to this list.
add_executable(SmartSearch
    main.cpp
    ConsoleUI.cpp
    SearchHistory.cpp
    AppServices.cpp
    # DocumentManager.cpp
    # SearchEngine.cpp
    # Autocomplete.cpp
)

if(MSVC)
    target_compile_options(SmartSearch PRIVATE /W4)
else()
    target_compile_options(SmartSearch PRIVATE -Wall -Wextra)
endif()
