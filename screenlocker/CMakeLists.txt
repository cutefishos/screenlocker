set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories( ${UNIXAUTH_INCLUDE_DIRS} )

set(PROJECT_SOURCES
    main.cpp
    application.cpp
    authenticator.cpp
    kcheckpass-enums.h
    fixx11h.h
    pamauthentication.cpp
    qml.qrc
)

add_executable(cutefish-screenlocker
    ${PROJECT_SOURCES}
)

target_link_libraries(cutefish-screenlocker
    PRIVATE
    Qt5::Core
    Qt5::DBus
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::Quick

    ${X11_LIBRARIES}
    ${UNIXAUTH_LIBRARIES}
    ${SOCKET_LIBRARIES}
)

install(TARGETS cutefish-screenlocker RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
