QT += widgets

INCLUDEPATH += include include/gui

SOURCES += \
    source/main.cpp \
    source/modifier.cpp \
    source/move.cpp \
    source/pokemon.cpp \
    source/pokemondb.cpp \
    source/stats.cpp \
    source/turn.cpp \
    source/gui/mainwindow.cpp \
    source/gui/movewindow.cpp \
    source/gui/resultwindow.cpp \
    source/item.cpp

HEADERS += \
    include/abilities.hpp \
    include/items.hpp \
    include/modifier.hpp \
    include/move.hpp \
    include/moves.hpp \
    include/pokemon.hpp \
    include/pokemondb.hpp \
    include/stats.hpp \
    include/turn.hpp \
    include/types.hpp \
    include/gui/mainwindow.hpp \
    include/gui/movewindow.hpp \
    include/gui/resultwindow.hpp \
    include/item.hpp

RESOURCES += \
    resources.qrc

