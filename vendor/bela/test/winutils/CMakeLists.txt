##

add_executable(pathcat_test
  pathcat.cc
)

target_link_libraries(pathcat_test
  belawin
)

add_executable(which
  which.cc
)

add_executable(env2
  env2.cc
)


target_link_libraries(which
  belawin
)

add_executable(expandenv
  expandenv.cc
)

target_link_libraries(expandenv
  belawin
)

target_link_libraries(env2
  belawin
)