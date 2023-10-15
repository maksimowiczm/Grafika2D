add_library(netpbm
        src/PixMap3.c
        src/PixMapReader.c
        src/PixMap6.c)

target_include_directories(netpbm PUBLIC include)
