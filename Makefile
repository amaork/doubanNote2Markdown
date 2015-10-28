CC			=	g++
CPPFLAGS	=	-Wall -g  
TARGETS		=	xml_to_md note_manager_test
SOURCES		=	note_manager.cpp xml_to_md.cpp note_manager_test.cpp xmlParser/xmlParser.cpp xmlParser/tinyxml2.cpp


all:$(TARGETS)
clean:
	make -C xmlParser clean
	rm -rf *.o $(TARGETS) a.out depend.d 

xml_to_md:	xml_to_md.o	note_manager.o xmlParser/xmlParser.o xmlParser/tinyxml2.o
note_manager_test:	note_manager_test.o note_manager.o	

depend.d:$(SOURCES)
	$(CC) -M $(CPPFLAGS) $^ > $@

-include depend.d

