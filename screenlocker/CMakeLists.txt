set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
    main.cpp
    application.cpp
    authenticator.cpp
    wallpaper.cpp
    kcheckpass-enums.h
    fixx11h.h
    qml.qrc
)

add_executable(screenlocker
    ${PROJECT_SOURCES}
)

target_link_libraries(screenlocker
    PRIVATE
    Qt5::Core
    Qt5::DBus
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::Quick

    ${X11_LIBRARIES}
)