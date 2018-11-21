function(setup_glad_glfw executable)
  # for linux use pkg-config
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
  message(STATUS "GLFW libraries found: " ${GLFW_LIBRARIES})
  message(STATUS "GLFW include dir found: " ${GLFW_INCLUDE_DIR})

  target_include_directories(${executable} PUBLIC
    ${GLFW_INCLUDE_DIR}
    glad/include
  )

  target_link_libraries(${executable} PUBLIC
    ${GLFW_LIBRARIES}
    dl # libdl needed for glad's dynamic linking
  )
endfunction(setup_glad_glfw)