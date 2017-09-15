SET(SRV_SRC
    main.cpp
    DB.cpp
    Session.cpp)

FILE(GLOB SRV_H
    *.h
    *.hpp
    )

FIND_LIBRARY(SQLITE3_LIBRARIES NAMES sqlite3)
FIND_PATH(SQLITE3_INCLUDE_DIR NAMES sqlite3.h)

INCLUDE_DIRECTORIES(../common/include ${SQLITE3_INCLUDE_DIR})
link_directories(/home/oskar/Programowanie/Hanasu/lib/)

ADD_EXECUTABLE(${SERVER} ${SRV_SRC} ${SRV_H})

TARGET_LINK_LIBRARIES(${SERVER} hanasu ${SQLITE3_LIBRARIES})
