PROJECT(active)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

AUX_SOURCE_DIRECTORY(. DIR_SRC)

SET(header_files
    active.h
    active_server.h
    mirco_active.h
)

SET(SRC_LIST 
    ${DIR_SRC}
)

ADD_LIBRARY(active ${SRC_LIST} ${header_files})