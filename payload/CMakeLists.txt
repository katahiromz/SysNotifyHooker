add_library(payload SHARED payload.cpp msgdump.cpp)
target_compile_definitions(payload PRIVATE PAYLOAD_BUILD)
set_target_properties(payload PROPERTIES PREFIX "")
set_target_properties(payload PROPERTIES OUTPUT_NAME ${PAYLOAD_NAME})
target_link_libraries(payload imagehlp)
