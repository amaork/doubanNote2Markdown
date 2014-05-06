#include <iostream>
#include <fstream>
#include <string>
#include "xml_db.h"
#include "xml_parser.h"

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

//	int flag = 0;
	NOTE_INFO ndata;
	XML_PARSER	*parser = NULL;
	unsigned int note_idx = 0;
	const char *book_name = NULL;

	/* Find context start and end */
	string ref_start_key(REF_START_KEY);
	string ref_end_key(REF_STOP_KEY);
	string all_note, context, remark;
	string::size_type context_start = string::npos;
	string::size_type context_end   = string::npos;

	/* Check params */
	if (argc != 3){

		fprintf(stdout, "Usage:%s input_file_name(*.xml) output_file_name(*.md)\n", argv[0]);
		return 0;
	}

	/* Markdown format output */
	ofstream markdown(argv[2], ofstream::out);

	if (!markdown){

		fprintf(stderr, "Open markdown file error!\n");
		return -1;
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

	/*	First level process book  */
	for (book = root->FirstChildElement(BOOK_KEY); book; book = book->NextSiblingElement(BOOK_KEY)){

		/* Find book name */	
		if ((book_name = book->Attribute(BOOK_NAME_ATTR)) == NULL){

			fprintf(stderr, "UNfound books name!\n");
			continue;
		}

		/* Write book name to markdown */
		markdown << "# " << book_name << endl;

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
			//parser->print(flag, cerr);
						
			/*	Parser note */
			if (!parser->parse()){

				/* Parser error, delete and continue */
				fprintf(stderr, "Parser book[%s] note[%d] error!\n", book_name, note_idx);
				delete parser;
				continue;
			}

			/* 	Process context and remark */
			all_note = string((const char*)ndata.note);

			/* Find start and end */
			context_start = all_note.find(ref_start_key);
			context_end   = all_note.find(ref_end_key);

			/* Split note as context and remark */
			if (context_start != string::npos && context_end != string::npos){
	
				context = all_note.substr(context_start + ref_start_key.size(), context_end - ref_end_key.size() + 1);
				remark  = all_note.substr(context_end + ref_end_key.size());
			}
			/* Only have remark, do not have context */
			else if (context_start == context_end && context_start == string::npos){

				context = all_note;
			}
		
			/*	Write note to markdown */
			markdown << "## [P" << ndata.page << "](" << ndata.url << ")" << endl;

			/* Context */
			markdown << context << endl << endl;

			/* Remark */
			markdown << "<font color=#7f7f7f>" << endl;
			markdown << remark << endl << endl <<endl;
			markdown << "</font>" << endl;
		}

	}

	markdown.flush();
	markdown.close();

	return 0;
}
