QT += widgets concurrent

INCLUDEPATH += include include/gui
LIBS += -ltinyxml2

SOURCES += \
    source/main.cpp \
    source/modifier.cpp \
    source/move.cpp \
    source/pokemon.cpp \
    source/pokemondb.cpp \
    source/stats.cpp \
    source/turn.cpp \
    source/gui/mainwindow.cpp \
    source/gui/resultwindow.cpp \
    source/item.cpp \
    source/gui/alertwindow.cpp \
    source/defenseresult.cpp \
    source/attackresult.cpp \
    source/gui/defensemovewindow.cpp \
    source/gui/attackmovewindow.cpp \
    source/gui/presetwindow.cpp \
    source/gui/addpresetwindow.cpp

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
    include/gui/resultwindow.hpp \
    include/item.hpp \
    include/gui/alertwindow.hpp \
    include/defenseresult.hpp \
    include/attackresult.hpp \
    include/gui/defensemovewindow.hpp \
    include/gui/attackmovewindow.hpp \
    include/gui/presetwindow.hpp \
    include/gui/addpresetwindow.hpp

RESOURCES += \
    resources.qrc

