include(GenerateExportHeader)

set(SOURCES
	vector.c
	bitwise_array.c
	lptr.c
	str.c)
set(CMAKE_SHARED_LIBRARY_ENABLE_EXPORTS True)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(adt SHARED ${SOURCES})
add_library(adtstatic STATIC ${SOURCES})

generate_export_header(adt
	BASE_NAME libadt)

target_include_directories(adt
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(adtstatic
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS adt adtstatic
	DESTINATION lib)
install(DIRECTORY libadt DESTINATION include)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libadt_export.h
	DESTINATION include/libadt)
install(FILES libadt.h DESTINATION include)
