add_executable(injector injector.cpp injector_res.rc)
target_link_libraries(injector payload)
set_target_properties(injector PROPERTIES OUTPUT_NAME ${INJECTOR_NAME})
