set(SOURCES
	vector.c
	bitwise_array.c
	lptr.c
	str.c)

add_library(adt SHARED ${SOURCES})
add_library(adtstatic STATIC ${SOURCES})

target_include_directories(adt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(adtstatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS adt adtstatic
	DESTINATION lib)
install(DIRECTORY libadt DESTINATION include)
install(FILES libadt.h DESTINATION include)
