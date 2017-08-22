cmake_minimum_required(VERSION 3.0)
project(blognetprotpar)
file(GLOB headers *.h)
file(GLOB sources *.cpp)
add_executable(
	${PROJECT_NAME}
	${headers}
	${sources}
)
target_link_libraries(
	${PROJECT_NAME}
	Ws2_32.lib
)