function(setup_glad_glfw executable)
  target_include_directories(${executable} PUBLIC
    glad/include
  )

  target_link_libraries(${executable} PUBLIC
    glfw
    dl # libdl needed for glad's dynamic linking
  )
endfunction(setup_glad_glfw)