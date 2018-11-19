function(setup_sfml executable)
  find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
  message(STATUS "SFML libraries found: " ${SFML_LIBRARIES})
  message(STATUS "SFML include dir found: " ${SFML_INCLUDE_DIR})

  target_include_directories(${PROJECT_NAME} PUBLIC
    ${SFML_INCLUDE_DIR}
  )

  target_link_libraries(${PROJECT_NAME} PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
  )
endfunction(setup_sfml)