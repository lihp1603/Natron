#boost: LIBS += -L/usr/local/lib/ -lboost_serialization -lboost_system -lboost_thread
boost {
        # Building boost from source (git clone, make, make install) is installed in /usr/local/lib
        PKGCONFIG -= boost
        LIBS -=  $$system(pkg-config --variable=libdir boost)/libboost_serialization.so
		LIBS -=  $$system(pkg-config --variable=libdir boost)/libboost_system.so 
		LIBS -=  $$system(pkg-config --variable=libdir boost)/libboost_thread.so
        LIBS += /usr/local/lib/libboost_serialization.so 
		LIBS +=/usr/local/lib/libboost_system.so 
		LIBS +=/usr/local/lib/libboost_thread.so
}
expat: LIBS += -lexpat
expat: PKGCONFIG -= expat
cairo: PKGCONFIG -= cairo
PYSIDE_PKG_CONFIG_PATH = $$system($$PYTHON_CONFIG --prefix)/lib/pkgconfig
pyside: PKGCONFIG += pyside
pyside: INCLUDEPATH += $$system(env PKG_CONFIG_PATH=$$PYSIDE_PKG_CONFIG_PATH pkg-config --variable=includedir pyside)/QtCore
pyside: INCLUDEPATH += $$system(env PKG_CONFIG_PATH=$$PYSIDE_PKG_CONFIG_PATH pkg-config --variable=includedir pyside)/QtGui
