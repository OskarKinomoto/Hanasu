SET(CLIENT_SRC
    main.cpp)

SET(CLIENT_H
    )

INCLUDE_DIRECTORIES(../common/include)

ADD_EXECUTABLE(${CLIENT} ${CLIENT_SRC} ${CLIENT_H})

TARGET_LINK_LIBRARIES(${CLIENT} ${COMMON})
