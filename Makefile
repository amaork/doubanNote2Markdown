CC		=	gcc
CPP		=	g++
CFLAGS	=	-Wall -g  
TARGETS	=	xml_to_md note_manager_test


all:$(TARGETS)
clean:
	rm -rf *.o $(TARGETS) a.out 

xml_to_md:	xml_to_md.o	tinyxml2.o xml_parser.o note_manager.o $(wildcard ./*.h)
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $@ $^

note_manager_test:	note_manager_test.o note_manager.o	
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $@ $^


xml_to_md.o:	xml_to_md.cpp
	$(CPP) $(CFLAGS) $(LDFLAGS) -c -o $@ $^


