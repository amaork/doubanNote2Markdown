CC		=	gcc
CPP		=	g++
CFLAGS	=	-Wall -g -D_DEBUG_ 
TARGETS	=	xml_to_md


all:$(TARGETS)
clean:
	rm -rf *.o $(TARGETS)  

xml_to_md:	xml_to_md.o	tinyxml2.o xml_parser.o	
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $@ $^


