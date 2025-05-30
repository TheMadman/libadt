include(GenerateExportHeader)

set(SOURCES
	vector.c
	bitwise_array.c
	lptr.c
	str.c)
set(CMAKE_SHARED_LIBRARY_ENABLE_EXPORTS True)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(adtobj OBJECT ${SOURCES})
add_library(adt SHARED)
target_link_libraries(adt adtobj)
add_library(adtstatic STATIC)
target_link_libraries(adtstatic adtobj)

generate_export_header(adt
	BASE_NAME libadt)

target_include_directories(adtobj
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS adt adtstatic
	DESTINATION lib)
install(DIRECTORY libadt DESTINATION include)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libadt_export.h
	DESTINATION include/libadt)
install(FILES libadt.h DESTINATION include)
