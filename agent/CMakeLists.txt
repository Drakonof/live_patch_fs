add_executable(livepatch_agent
    main.cpp
)

target_link_libraries(livepatch_agent PRIVATE
    livepatch_proto
)
