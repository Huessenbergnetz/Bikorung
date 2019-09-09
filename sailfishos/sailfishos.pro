TARGET = harbour-bikorung

!contains(CONFIG, clazy) {
    CONFIG += sailfishapp
}
CONFIG += c++11
CONFIG += c++14

QT += network

VER_MAJ = 0
VER_MIN = 0
VER_PAT = 1

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

DEFINES += VERSION_STRING=\"\\\"$${VERSION}\\\"\"

CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

contains(CONFIG, clazy) {
    DEFINES += CLAZY
    isEmpty(CLAZY_PLUGIN_FILE): CLAZY_PLUGIN_FILE = ClazyPlugin.so
    QMAKE_CXXFLAGS += "-Xclang -load -Xclang $${CLAZY_PLUGIN_FILE} -Xclang -add-plugin -Xclang clazy -Xclang -plugin-arg-clazy -Xclang level0,level1,level2,reserve-candidates,qrequiredresult-candidates,qvariant-template-instantiation"
    QT += qml quick
    CONFIG += link_pkgconfig
}

contains(CONFIG, asan) {
    QMAKE_CXXFLAGS += "-fsanitize=address -fno-omit-frame-pointer -Wformat -Werror=format-security -Werror=array-bounds -g -ggdb"
    QMAKE_LFLAGS += "-fsanitize=address"
}

PKGCONFIG += openssl
!contains(CONFIG, clazy) {
    PKGCONFIG += nemonotifications-qt5
    INCLUDEPATH += /usr/include/nemonotifications-qt5
}

SOURCES += \
    src/main.cpp \
    src/licensesmodel.cpp \
    src/languagesmodel.cpp

HEADERS += \
    src/licensesmodel.h \
    src/languagesmodel.h

DISTFILES += qml/harbour-bikorung.qml \
    qml/cover/CoverPage.qml \
    qml/dialogs/AddEditServer.qml \
    qml/pages/MainPage.qml \
    qml/pages/About.qml \
    qml/pages/Settings.qml \
    qml/models/ChangelogModel.qml \
    qml/models/ContributorsModel.qml \
    qml/parts/TextFieldWithDesc.qml \
    rpm/harbour-bikorung.changes \
    rpm/harbour-bikorung.spec \
    rpm/harbour-bikorung.yaml \
    translations/*.ts \
    harbour-bikorung.desktop

include(../common/common.pri)

SAILFISHAPP_ICONS = 86x86 108x108 128x128 150x150 172x172

isEmpty(INSTALL_TRANSLATIONS_DIR): INSTALL_TRANSLATIONS_DIR = /usr/share/$$TARGET/translations

DEFINES += TRANSLATIONS_DIR=\"\\\"$${INSTALL_TRANSLATIONS_DIR}\\\"\"

langfiles.path = $$INSTALL_TRANSLATIONS_DIR
langfiles.files = ../translations/*.qm
INSTALLS += langfiles

icons.path = /usr/share/$$TARGET/icons
icons.files = icons/z*
INSTALLS += icons

include(../HBN_SFOS_Components/HBN_SFOS_Components.pri)
