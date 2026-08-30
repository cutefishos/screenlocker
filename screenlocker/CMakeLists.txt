set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
    main.cpp
    application.cpp
    authenticator.cpp
    kcheckpass-enums.h
    fixx11h.h
    qml.qrc
)

add_executable(cutefish-screenlocker
    ${PROJECT_SOURCES}
)

target_link_libraries(cutefish-screenlocker
    PRIVATE
    Qt6::Core
    Qt6::DBus
    Qt6::Widgets
    Qt6::Quick

    ${X11_LIBRARIES}
)

install(TARGETS cutefish-screenlocker RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
