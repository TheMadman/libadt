set(SOURCES
	vector.c
	bitwise_array.c
	lptr.c
	str.c)

add_library(adtobj OBJECT ${SOURCES})
add_library(adt SHARED)
target_link_libraries(adt adtobj)
add_library(adtstatic STATIC)
target_link_libraries(adtstatic adtobj)

target_include_directories(adtobj
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS adt adtstatic
	DESTINATION lib)
install(DIRECTORY libadt DESTINATION include)
install(FILES libadt.h DESTINATION include)
