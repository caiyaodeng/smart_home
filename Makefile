# target, subdir, objects in current dir
TARGET	= serv_smart_home 
SUBDIRS	= base \
		  db/mysql \
		  net \
		  smart_home
OBJECTS	= 


all:subdirs ${OBJECTS}
	${CC} -o ${TARGET} $$(find ./${SUBDIRS} -name '*.o') ${LDFLAGS} ${INCLUDES} $(LIBSHAREPATH)


clean:cleansubdirs
	rm -f ${TARGET} ${OBJECTS}


# path of "make global scripts"
# NOTE, use absolute path. export once, use in all subdirs
export PROJECTPATH=${PWD}
export MAKEINCLUDE=${PROJECTPATH}/Makefile.in

# include "make global scripts"
include ${MAKEINCLUDE}
