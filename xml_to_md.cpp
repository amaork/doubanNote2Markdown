#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "xml_db.h"
#include "xml_parser.h"
#include "note_manager.h"

typedef struct{
	unsigned int page;
	unsigned char url[128];
	unsigned char note[0x4000];
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
	XML_PARSER	*parser = NULL;
	unsigned int note_idx = 0;
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
		
			/*	Find note idx */
			if (!(note_idx = note->IntAttribute(NOTE_IDX_ATTR))){

				fprintf(stderr, "UNfound note id!\n");
				continue;
			}

			/*	Create a parser */	
			parser = new XML_PARSER(book_name, note);		

			/*	Add parser rules to parser */	
			parser->add_rule(URL_KEY, ndata.url);		
			parser->add_rule(PAGE_KEY, &ndata.page);
			parser->add_rule(CONTENT_KEY, ndata.note);

			/*	Print parser info */
			//cout << *parser << endl;
						
			/*	Parser note */
			if (!parser->parse(false, cout)){

				/* Parser error, delete and continue */
				fprintf(stderr, "Parser book[%s] note[%d] error!\n", book_name, note_idx);
				delete parser;
				continue;
			}

			/* Add note to note manager */
			note_manager->add_note(book_name, Note((const char*)ndata.note, (const char*)ndata.url, ndata.page));
		}

	}

	/* Markdown out put */
	note_manager->markdown_output(string(argv[1]).append(".md"));	

	delete note_manager;

	return 0;
}
