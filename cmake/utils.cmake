function(copy_to_bin_dir)
  foreach(filename ${ARGV})
    file(COPY ${filename} DESTINATION ${CMAKE_BINARY_DIR})    
  endforeach(filename)  
endfunction(copy_to_bin_dir)