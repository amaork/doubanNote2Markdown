#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "xml_db.h"
#include "note_manager.h"
#include "xmlParser/xmlParser.h"

typedef struct{
	int page;
	char url[128];
	char note[0x4000];
}NOTE_INFO, *P_NOTE_INFO;

using namespace std;
using namespace tinyxml2;


/* Main functions */
int main(int argc, char **argv)
{
	XMLDocument xml;
	XMLElement* root = NULL;
	XMLElement* book = NULL;
	XMLElement* note = NULL;
	XMLPrinter printer(stderr);

	NOTE_INFO ndata;
	int note_idx = 0;
	xmlParser	*parser = NULL;
	const char *book_name = NULL;
	Note_Manager *note_manager = NULL;

	/* Check params */
	if (argc != 2){

		fprintf(stdout, "Usage:%s input_file_name(*.xml)\n", argv[0]);
		return 0;
	}

	/* Load note from xml file anc check */
	if (xml.LoadFile(argv[1]) != XML_NO_ERROR){

		fprintf(stderr, "Load user note file error %s:%s\n", xml.GetErrorStr1(), xml.GetErrorStr2());
		return -1;
	}

	/*	Find root element */
	if ((root = xml.RootElement()) == NULL){

		fprintf(stderr, "Do not find xml root node!\n");
		return -1;
	}

	/* Alloc a note manager */
	note_manager = new Note_Manager;

	/*	First level process book  */
	for (book = root->FirstChildElement(BOOK_KEY); book; book = book->NextSiblingElement(BOOK_KEY)){

		/* Find book name */	
		if ((book_name = book->Attribute(BOOK_NAME_ATTR)) == NULL){

			fprintf(stderr, "UNfound books name!\n");
			continue;
		}

		/* Second level process each book note */
		for (note = book->FirstChildElement(NOTE_KEY); note; note = note->NextSiblingElement(NOTE_KEY)){

			/*	Clean data */
			bzero(&ndata, sizeof(NOTE_INFO));

			/*	Create a parser */	
			parser = new xmlParser(book_name);		

			/*	Add parser rules to parser */	
			parser->addRule(xmlParseRule(URL_KEY, ndata.url));		
			parser->addRule(xmlParseRule(PAGE_KEY, &ndata.page));
			parser->addRule(xmlParseRule(CONTENT_KEY, ndata.note));
			parser->addAttrRule(xmlParseRule(NOTE_IDX_ATTR, &note_idx));
		
			/*	Find note idx */
			if (!parser->parseAttr(note)){

				fprintf(stderr, "UNfound note id!\n");
				continue;
			}

			/*	Print parser info */
			//cout << *parser << endl;
			//cout << note_idx << ":" << book_name << endl;
						
			/*	Parser note */
			if (!parser->parseValue(note)){

				/* Parser error, delete and continue */
				fprintf(stderr, "Parser book[%s] note[%d] error!\n", book_name, note_idx);
				delete parser;
				continue;
			}

			/* Add note to note manager */
			note_manager->add_note(book_name, Note((const char*)ndata.note, (const char*)ndata.url, ndata.page));

			/* Delte parser */
			delete parser;
		}

	}

	/* Markdown out put */
	note_manager->markdown_output(string(argv[1]));	

	delete note_manager;

	return 0;
}
