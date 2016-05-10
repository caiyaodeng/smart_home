# subdir and objects in current dir
SUBDIRS	= 
OBJECTS	= bll.o \
		  dal.o \
		  id_identifier.o \
		  message_parser.o \
		  message_shunter.o \
		  polling_machine.o \
		  ready_device.o \
		  ready_user.o \
		  srv.o \
		  transit_center.o \
		  undo_task.o \
		  user_configure_updater.o


all:subdirs ${OBJECTS}

	
clean:cleansubdirs
	rm -f ${OBJECTS}
	
include ${MAKEINCLUDE}
