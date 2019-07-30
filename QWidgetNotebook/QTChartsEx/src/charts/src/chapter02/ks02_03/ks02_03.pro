include ($$(TRAINDEVHOME)/src/gui_base.pri)

TEMPLATE	= app
LANGUAGE	= C++

#CONFIG		+= console
QT			+= widgets charts

TEMPDIR		= $$TRAIN_OBJ_PATH/chapter02/ks02_03


#INCLUDEPATH += $$TRAIN_INCLUDE_PATH
		
FORMS		+= dialogbase.ui

HEADERS		+= $$TRAIN_SRC_PATH/gui_base.pri \
                                ks02_03.pro \
				dialog.h
	
	
SOURCES    +=  main.cpp \
				dialog.cpp
				
				debug_and_release {
	CONFIG(debug, debug|release) {	
	
                TARGET = ks02_03_d
	}
	CONFIG(release, debug|release) {			   
				   
                TARGET	= ks02_03
	}
} else {
	debug {
	
                TARGET	= ks02_03_d
	}
	release {
		
                TARGET 	= ks02_03
	}
}

DESTDIR     = $$TRAIN_BIN_PATH
OBJECTS_DIR = $$TEMPDIR
MOC_DIR		= $$TEMPDIR/moc
UI_DIR		= $$TEMPDIR/ui
