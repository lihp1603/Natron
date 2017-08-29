boost: LIBS += -lboost_serialization -lboost_system -lboost_thread
PKGCONFIG += expat
PKGCONFIG += fontconfig
cairo {
        PKGCONFIG += cairo
        LIBS -=  $$system(pkg-config --variable=libdir cairo)/libcairo.a
}
pyside {
        PKGCONFIG -= pyside
        INCLUDEPATH += $$system(pkg-config --variable=includedir pyside)
        INCLUDEPATH += $$system(pkg-config --variable=includedir pyside)/QtCore
        INCLUDEPATH += $$system(pkg-config --variable=includedir pyside)/QtGui
        INCLUDEPATH += $$system(pkg-config --variable=includedir QtGui)
        LIBS += -lpyside-python2.7
}
shiboken {
        PKGCONFIG -= shiboken
        INCLUDEPATH += $$system(pkg-config --variable=includedir shiboken)
        LIBS += -lshiboken-python2.7
}
