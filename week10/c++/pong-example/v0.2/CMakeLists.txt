cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Set project name and version
project(PongSpire VERSION 0.2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Enable debug symbols by default
if(NOT CMAKE_BUILD_TYPE)
  #set(CMAKE_BUILD_TYPE Debug 
  set(CMAKE_BUILD_TYPE Release
    CACHE STRING 
      "Choose the type of build (Debug or Release)" 
      FORCE
    )
endif()

# Generate config.h
configure_file(include/config.h.in config.h)

# Find SFML shared libraries
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

# Compile executable
add_executable(PongSpire src/main.cpp src/Paddle.cpp src/Ball.cpp)

# Set include directory search paths
target_include_directories(PongSpire PRIVATE 
	"${PROJECT_BINARY_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Set executable RPATH
set_target_properties(PongSpire
  PROPERTIES
      INSTALL_RPATH
        "${CMAKE_INSTALL_PREFIX}/lib"
  )

# Link executable to required SFML libraries
target_link_libraries(PongSpire sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Install target
install(TARGETS PongSpire DESTINATION bin)
