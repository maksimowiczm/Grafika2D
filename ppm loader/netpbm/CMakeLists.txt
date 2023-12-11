add_library(netpbm
        src/PixMap.c
        src/PixMap3.c
        src/PixMap6.c
        src/PixMapReader.c)

target_include_directories(netpbm PUBLIC include)
