#ifndef	_XML_PARSER_H_
#define _XML_PARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

/* XML parse rule */
class XML_PARSE_RULE{

	public:

		/*	Constructor */
		XML_PARSE_RULE(){i_value = NULL, c_value = NULL, is_str = false;}
		XML_PARSE_RULE(const char *k, unsigned int *v) {key = k, i_value = v, is_str = false;}
		XML_PARSE_RULE(const char *k, unsigned char *v){key = k, c_value = v, is_str = true;}		

		/*	Check this rule is valid */
		bool check(void);
		
		/*	Print this rule */
		void print(ostream &os = cout) {os << "is_str\t" << is_str << "\tkey:\t" << key << endl;}
		

		/* Data */
		string key;
		bool is_str;
		unsigned int *i_value;
		unsigned char *c_value;	
};

/* XML parser */
class XML_PARSER{

	public:

		/*	Constructor */
		XML_PARSER(const string &desc, bool debug = false);
		XML_PARSER(const string &desc, XMLElement *node, bool debug = false);

		/*	Parse xml */
		bool parse(bool debug = false, ostream &debug_os = cerr);

		/*	Dispaly parser info */
		void print(int &flag, ostream &os = cout);

		/*	Debug opetion */
		void set_debug(bool st) {debug_option = st; }

		/* 	Set xml root node */	
		void set_root(XMLElement *node){ root = node;}

		/*	Set parser desc */
		void set_desc(const string &desc) {parser_desc = desc;}

		/*	Add  xml parsing rules */
		void add_rule(const XML_PARSE_RULE &rule);
		void add_rule(const char *key, unsigned int *value);
		void add_rule(const char *key, unsigned char *value);

	private:

		/*	Check parser */
		bool check(void);

		/*	xml parser root node */
		XMLElement *root;

		/*	Debug options */
		bool debug_option;

		/*	xml parser desc */
		string parser_desc;

		/*	xml parse rules */
		vector< XML_PARSE_RULE> parser_rules;
};

#endif
