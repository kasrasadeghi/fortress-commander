function(setup_gtest executable)
  find_package(GTest REQUIRED)
  message(STATUS "GTest libraries found: " ${SFML_LIBRARIES})
  message(STATUS "GTest include dir found: " ${SFML_INCLUDE_DIR})

  target_include_directories(${executable} PUBLIC
    ${SFML_INCLUDE_DIR}
  )

  target_link_libraries(${executable} PUBLIC
    gtest
    gtest_main
  )

  enable_testing()
endfunction(setup_gtest)