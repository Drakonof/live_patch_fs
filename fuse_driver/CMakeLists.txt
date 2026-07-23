add_executable(livepatch_fuse
    main.cpp
    LivePatchFs.cpp
)

target_link_libraries(livepatch_fuse PRIVATE
    fuse3
    pthread
    stdc++fs
)
