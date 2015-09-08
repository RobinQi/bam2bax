include_directories(
    .
    ${BLASR_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${HDF5_INCLUDE_DIRS}
    ${HTSLIB_INCLUDE_DIRS}
    ${PacBioBAM_INCLUDE_DIRS}
    ${PBDATA_INCLUDE_DIRS}
    ${PBDATA_ROOT_DIR}
    ${PBIHDF_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
)

set(SOURCES
    main.cpp
    Settings.h
    Settings.cpp
    IConverter.h
    IConverter.cpp
    Bam2BaxInternal.h
    RegionTypeAdapter.h
    RegionsAdapter.h
    Bam2BaxConverter.h
    Bam2BaxConverter.cpp
    Bam2Bax.h
    Bam2Bax.cpp
    OptionParser.h
    OptionParser.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Bam2Bax_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${Bam2Bax_EXE_LINKER_FLAGS}")

add_executable(bam2bax ${SOURCES})
set_target_properties(bam2bax PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${Bam2Bax_BinDir}
)
target_link_libraries(bam2bax
    ${BLASR_LIBRARIES}
    ${PBIHDF_LIBRARIES}
    ${PBDATA_LIBRARIES}
    ${HDF5_LIBRARIES}
    ${HDF5_CPP_LIBRARIES}
    ${PacBioBAM_LIBRARIES}
    ${HTSLIB_LIBRARIES}
    ${ZLIB_LIBRARIES}
)
