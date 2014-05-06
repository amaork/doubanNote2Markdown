CC		=	gcc
CPP		=	g++
CFLAGS	=	-Wall -g  
TARGETS	=	xml_to_md


all:$(TARGETS)
clean:
	rm -rf *.o $(TARGETS)  

xml_to_md:	xml_to_md.o	tinyxml2.o xml_parser.o $(wildcard ./*.h)
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $@ $^

xml_to_md.o:	xml_to_md.cpp
	$(CPP) $(CFLAGS) $(LDFLAGS) -c -o $@ $^
