########################################################################
# Template projectfile for pvbrowser server using Python 
# Attention: Please adjust "/usr/lib64/libpython2.5.so" for your system
########################################################################

TEMPLATE = app
CONFIG  += warn_on release console
CONFIG  -= qt

# Input
HEADERS += pvapp.h      \
           mask1_slots.h
SOURCES += main.cpp     \
           mask1.cpp

!macx {
unix:LIBS          += /usr/lib/libpvsmt.so -lpthread /usr/lib64/libpython2.7.so
#unix:LIBS         += /usr/lib/libpvsid.so
unix:INCLUDEPATH   += /opt/pvb/pvserver /usr/include/python2.7
unix:LIBS          += /usr/lib/librllib.so
unix:INCLUDEPATH   += /opt/pvb/rllib/lib
}

# python not tested on Mac OS X jet. Please try yourself by adding the python lib below.
macx:LIBS          += /opt/pvb/pvserver/libpvsmt.a /usr/lib/libpthread.dylib
#macx:LIBS         += /opt/pvb/pvserver/libpvsid.a
macx:INCLUDEPATH   += /opt/pvb/pvserver
macx:LIBS          += /usr/lib/librllib.dylib
macx:INCLUDEPATH   += /opt/pvb/rllib/lib

win32-g++ {
QMAKE_LFLAGS       += -static-libgcc
win32:INCLUDEPATH  += $(PVBDIR)/pvserver
win32:INCLUDEPATH  += $(PYDIR)/include
win32:LIBS         += $(PVBDIR)/win-mingw/bin/_pv.dll 
win32:LIBS         += $(PVBDIR)/win-mingw/bin/_rllib.dll 
win32:LIBS         += $(PVBDIR)/win-mingw/bin/librllib.a
win32:INCLUDEPATH  += $(PVBDIR)/rllib/lib
win32:LIBS         += $(PVBDIR)/win-mingw/bin/libserverlib.a -lws2_32 $(PYDIR)/libs/libpython25.a
}
else {
win32:LIBS         += $(PVBDIR)/win/bin/serverlib.lib wsock32.lib $(PYDIR)/libs/python25.lib
win32:INCLUDEPATH  += $(PVBDIR)/pvserver
win32:INCLUDEPATH  += $(PYDIR)/include
win32:LIBS         += $(PVBDIR)/win/bin/_pv.lib 
win32:LIBS         += $(PVBDIR)/win/bin/rllib.lib
win32:INCLUDEPATH  += $(PVBDIR)/rllib/lib
win32:LIBS         += $(PVBDIR)/win/bin/_rllib.lib
}

#DEFINES += USE_INETD
TARGET = pvs
