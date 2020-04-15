TEMPLATE	= app
CONFIG		= qt warn_on release
HEADERS		= toolbarimg.h \
		  toolbarimg_reverse.h \
		  zcm.h \
		  zcmimage.h
SOURCES		= canvas.cpp \
		  main.cpp \
		  mytoolbar.cpp \
		  zcm.cpp
INTERFACES	= 
DESTDIR = ./
INCLUDEPATH += $(QTDIR)/library
DEPENDPATH += $(QTDIR)/library
TARGET = zcm
LIBS += -lqpe -lsl
