function(setup_freetype executable)
  find_package(Freetype REQUIRED)
  message(STATUS "Freetype libraries found: " ${FREETYPE_LIBRARIES})
  message(STATUS "Freetype include dir found: " ${FREETYPE_INCLUDE_DIRS})

  target_include_directories(${executable} PUBLIC
    ${FREETYPE_INCLUDE_DIRS}
  )

  target_link_libraries(${executable} PUBLIC
    ${FREETYPE_LIBRARIES}
  )

  enable_testing()
endfunction(setup_freetype)