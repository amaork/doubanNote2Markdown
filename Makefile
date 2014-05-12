CC			=	g++
CPPFLAGS	=	-Wall -g  
TARGETS		=	xml_to_md note_manager_test
SOURCES		=	xml_parser.cpp note_manager.cpp xml_to_md.cpp note_manager_test.cpp


all:$(TARGETS)
clean:
	rm -rf *.o $(TARGETS) a.out depend.d 

xml_to_md:	xml_to_md.o	tinyxml2.o xml_parser.o note_manager.o
note_manager_test:	note_manager_test.o note_manager.o	

depend.d:$(SOURCES)
	$(CC) -M $(CPPFLAGS) $^ > $@

-include depend.d

