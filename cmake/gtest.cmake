function(setup_gtest executable)
  find_package(GTest REQUIRED)

  target_link_libraries(${executable} PUBLIC
    gtest
    gtest_main
  )

  enable_testing()
endfunction(setup_gtest)