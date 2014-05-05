#include "xml_parser.h"
#include <strings.h>
#include <algorithm>

/**************************************************************************************
	@brief	:	struct parse_rule operator== mothed	
**************************************************************************************/
bool operator== (const XML_PARSE_RULE &lhs, const XML_PARSE_RULE &hhs)
{
	return ((lhs.key == hhs.key) && (lhs.is_str == hhs.is_str));
}

/**************************************************************************************
	@brief	:	Constructor
**************************************************************************************/
XML_PARSER::XML_PARSER(const string &desc, bool debug)
{
	/* 	Set parser name */
	parser_desc = desc;
	
	/* 	Clear parser rules */
	parser_rules.clear();

	/*	Clear xml parser root */
	root = NULL;

	/*	Debug option is disabled */		
	debug_option	=	debug;	
}


XML_PARSER::XML_PARSER(const string &desc, XMLElement *node, bool debug)
{
	/* 	Set parser name */
	parser_desc = desc;
	
	/* 	Clear parser rules */
	parser_rules.clear();

	/*	Clear xml parser root */
	root = node;

	/*	Debug option is disabled */		
	debug_option	=	debug;	
}

/**************************************************************************************
	@brief	Add xml parse rule, value is an integer type 
**************************************************************************************/
void XML_PARSER::add_rule(const char *key, unsigned int *value)
{
	/*	Add new parse rule to parser rule linklist */	
	parser_rules.push_back(XML_PARSE_RULE(key, value));	
}

void XML_PARSER::add_rule(const char *key, unsigned char *value)
{
	/*	Add new parse rule to parser rule linklist */	
	parser_rules.push_back(XML_PARSE_RULE(key, value));	
}

void XML_PARSER::add_rule(const XML_PARSE_RULE &rule)
{
	parser_rules.push_back(rule);
}


/**************************************************************************************
	@brief	:	Parser self check 
**************************************************************************************/
bool XML_PARSER::check(void)
{
	/*	Check parser rules */
	if (parser_rules.empty()){

		cerr << "Parser[" << parser_desc << "] parse rules linklist is empty!" << endl;
		return false; 
	}

	/*	Check parser root node */
	if (!root){

		cerr << "Parser[" << parser_desc << "] parse root node unset!" << endl;
		return false;
	}

	return true;
}

bool XML_PARSE_RULE::check(void)
{
	/* 	Check key */
	if (!key.size()){

		cerr << "Rule error: key is empty!" << endl;
		return false;
	}
	
	/*	Check value target */	
	if ((is_str && !c_value) || (!is_str && !i_value)){

		cerr << "Rule error: value is invalid!" << endl;
		return false;
	}

	return true;
}


/**************************************************************************************
	@brief	:	Display parser infomaction 
**************************************************************************************/
void XML_PARSER::print(int &flag, ostream &os)
{
	vector< XML_PARSE_RULE>::iterator iter;

	/*	Debug off exit */
	if (!debug_option) return;

	/*	Check flag */
	if (flag) return;

	/*	Check parser at first */	
	if (!check()) return;

	os << "=============================================================" << endl; 
	os << "Parser info" << endl;
	os <<	"Name:\t\t" << parser_desc << endl;
	os << "Debug:\t\t"<< debug_option << endl;
	os << "Rules num:\t" << parser_rules.size() << endl;
	os << "Root node name:\t" << root->Value() << endl;

	/*	Find first rule */
	iter = parser_rules.begin();

	/*	Print every rules */
	while(iter != parser_rules.end()){
		
		iter->print(os);
		iter++;
	}

	/*	After print set flag marked */
	flag = 1;	
}

/**************************************************************************************
	@brief	:	Parsing xml according rules, after parsing delete the rule  
**************************************************************************************/
bool XML_PARSER::parse(bool debug, ostream &debug_os)
{
	XML_PARSE_RULE rule;
	XMLElement *key = NULL;
	const char *value = NULL;

	/*	Set debug option */
	debug_option = debug;

	/*	Check parser is ready */
	if (!check()) return false;

	if (debug)
		debug_os << "+++++++++++++++++++++++" << endl; 		
	
	/*	Once pop a parser rule from parser rules linklist   */
	while(!parser_rules.empty() && root){

		/*	Get a parser rule */
		rule = parser_rules.back();

		/*	Remove this rule from rules linklist */
		parser_rules.pop_back();

		/*	Check rule */
		if (!rule.check())continue;
	
		/* 	Find key */	   
		key = root->FirstChildElement(rule.key.c_str());	

		/* 	Key is find ? */
		if (!key){

			cerr << "Parser[" << parser_desc << "]\tUndefined key:" << rule.key << endl;
			continue;
		}

		/* 	Get key value */
		value = key->ToElement()->GetText();

		/* 	Find key value ? */
		if (!value){

			cerr << "Parser[" << parser_desc << "]\tKey[" << rule.key << "] value is empty!" << endl;
			continue;
		}
	
		/*	Read key value */
		if (rule.is_str && rule.c_value){

			/* Value is a string */
			bcopy(value, rule.c_value, strlen(value));
		}
		else if (rule.i_value){

			/* Value is a integer */
			*rule.i_value = atoi(value);
		}

		/* Debug output */	
		if (debug_option){

			if (rule.is_str){

				debug_os << rule.key << ":" << rule.c_value << endl;
			}
			else{

				debug_os << rule.key << ":" << *rule.i_value << endl;
			}
		}	

	} /* While */

	return true;
}

