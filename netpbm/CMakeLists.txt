add_library(netpbm
        src/PixMap3.c
        src/PixMapReader.c)

target_include_directories(netpbm PUBLIC include)
