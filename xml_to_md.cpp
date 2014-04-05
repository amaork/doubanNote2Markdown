#include <iostream>
#include <fstream>
#include "debug.h"
#include "xml_db.h"
#include "xml_parser.h"


typedef struct{
	unsigned int page;
	unsigned char url[128];
	unsigned char note[0x4000];
}NOTE_INFO, *P_NOTE_INFO;


using namespace std;
using namespace tinyxml2;

int main(int argc, char **argv)
{
	XMLDocument xml;
	XMLElement* root = NULL;
	XMLElement* book = NULL;
	XMLElement* note = NULL;
	XMLPrinter printer(stderr);

//	int flag = 0;
	NOTE_INFO 	ndata;
	XML_PARSER	*parser = NULL;
	unsigned int note_idx = 0;
	const char *book_name = NULL;

	/* Check params */
	if (argc != 3){

		__debug_mesg__("Usage:%s input_file_name(*.xml) output_file_name(*.md)\n", argv[0]);
		return 0;
	}

	/* Markdown format output */
	ofstream markdown(argv[2], ofstream::out);

	if (!markdown){

		__DEBUG_MESG__("Open markdown file error!");
		return -1;
	}

	/* Load note from xml file anc check */
	if (xml.LoadFile(argv[1]) != XML_NO_ERROR){

		__debug_mesg__("Load user note file error %s:%s\n", xml.GetErrorStr1(), xml.GetErrorStr2());
		return -1;
	}

	/*	Find root element */
	if ((root = xml.RootElement()) == NULL){

		__DEBUG_MESG__("Do not find xml root node!");
		return -1;
	}

	/*	First level process book  */
	for (book = root->FirstChildElement(BOOK_KEY); book; book = book->NextSiblingElement(BOOK_KEY)){

		/* Find book name */	
		if ((book_name = book->Attribute(BOOK_NAME_ATTR)) == NULL){

			__DEBUG_MESG__("UNfound books name!");
			continue;
		}

		/* Write book name to markdown */
		markdown << "# " << book_name << endl;

		/* Second level process each book note */
		for (note = book->FirstChildElement(NOTE_KEY); note; note = note->NextSiblingElement(NOTE_KEY)){

			/*	Clean data */
			bzero(&ndata, sizeof(ndata));
		
			/*	Find note idx */
			if (!(note_idx = note->IntAttribute(NOTE_IDX_ATTR))){

				__DEBUG_MESG__("UNfound note id!");
				continue;
			}

			/*	Create a parser */	
			parser = new XML_PARSER(book_name, note);		

			/*	Add parser rules to parser */	
			parser->add_rule(URL_KEY, ndata.url);		
			parser->add_rule(PAGE_KEY, &ndata.page);
			parser->add_rule(CONTENT_KEY, ndata.note);

			/*	Print parser info */
			//parser->print(flag, cerr);

			/*	Parser note */
			if (!parser->parse()){

				__debug_mesg__("Parser book[%s] note[%d] error!\n", book_name, note_idx);
				goto out;
			}

			/*	Write note to markdown */
			markdown << "## [P" << ndata.page << "](" << ndata.url << ")" << endl;
			markdown << ndata.note << endl << endl << endl;

			out:
			delete parser;
		}

	}

	markdown.flush();
	markdown.close();

	return 0;
}


