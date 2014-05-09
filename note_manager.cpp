#include <algorithm>
#include <fstream>
#include <sstream>
#include "xml_db.h"
#include "note_manager.h"

/**************************************************************************************
	@brief	:	Delete space, table, enter charactor at string before and end
**************************************************************************************/
string trim(string str)
{
	/* Delete space,table and enter  */
	const string del = " \t\n";

	/* Delte tail */
	string tmp = str.erase(str.find_last_not_of(del) +1);

	/* Delete head */
	return tmp.erase(0, tmp.find_first_not_of(del));
}


/**************************************************************************************
	@brief	:	Output context with url detected
**************************************************************************************/
void markdown_output_with_url_detect(ostream& out, const string &txt, const string &skey, const string &ekey)
{
	string url;
	string context = txt;
	string::size_type start, end;

	/* Process contexts */
	while (context.size()){

		/* Find url start */
		if ((start = context.find(skey)) != string::npos && (end = context.substr(start, context.size()).find(ekey)) != string::npos){

			/* Output the context before url start */
			if (start){
				out << context.substr(0, start) << endl;
			}

			/* Output url */
			url = context.substr(start, end + ekey.size());

			out << "![" << url << "](" << url << ")" << endl;

			/* Remove already process sting */
			context.erase(0, start + end + ekey.size());

			/* Process next */
			continue;
		}


		/* There's no url being found */
		out << context << endl;
		context.erase(0, context.size());
	
	} /* end of while */
}


/**************************************************************************************
	@brief	:	Note constructor
**************************************************************************************/
Note::Note(const char *note, const char *note_url, unsigned int note_page)
{
	string all_note = string(note);
	string ref_end_key(REF_STOP_KEY);
	string ref_start_key(REF_START_KEY);

	/* Find context start and end */
	string::size_type start = all_note.find(ref_start_key);
	string::size_type end   = all_note.find(ref_end_key); 
	
	/* Split note as context and remark */
	if (start != string::npos && end != string::npos){
	
		context = trim(all_note.substr(start + ref_start_key.size(), end - ref_end_key.size() + 1));
		remark  = trim(all_note.substr(end + ref_end_key.size()));
	}
	/* Only have remark, do not have context */
	else if (start == end && start == string::npos){

		remark = trim(all_note);
	}

	/* Copy url and page */
	url = string(note_url);
	page = note_page;
}

/**************************************************************************************
	@brief	:	Note overloaded output << operator
**************************************************************************************/
ostream &operator << (ostream& out, const Note &note)
{
	string jpg;
	string jpg_end_key(".jpg");
	string jpg_start_key("http://");

	/* Write page and url */
	out << "## [P" << note.page << "](" << note.url << ")" << endl;
			
	/* Write context */	
	if (note.context.size()){

		//out << note.context << endl << endl;
		markdown_output_with_url_detect(out, note.context, jpg_start_key, jpg_end_key);
	}

	/* Write remark */
	if (note.remark.size()){

		out << "<font color=#7f7f7f>" << endl;
		//out	<< note.remark << endl;
		markdown_output_with_url_detect(out, note.remark, jpg_start_key, jpg_end_key);
		out << "</font>" << endl;
	}

	return out;
}

/**************************************************************************************
	@brief	:	Comparison function to be uesd to sort by page number
**************************************************************************************/
bool is_page_number_little(const Note &n1, const Note &n2)
{
	return n1.page < n2.page;	
}

/**************************************************************************************
	@brief	:	Add a note to note manager
**************************************************************************************/
void Note_Manager::add_note(const string &book_name, Note &note)
{
	note_book[book_name].push_back(note);
}

void Note_Manager::add_note(const char *book_name, Note &note)
{
	add_note(string(book_name), note);
}

void Note_Manager::add_note(const char *book_name, Note *note)
{
	add_note(string(book_name), *note);
}

void Note_Manager::add_note(const char *book_name, Note note)
{
	add_note(string(book_name), note);
}

/**************************************************************************************
	@brief	:	Get book name list
**************************************************************************************/
Book_list Note_Manager::get_book_list(void)
{	
	Book_list book_list;
	Note_book_list::const_iterator note_book_it;

	/* Get all books name */
	for (note_book_it = note_book.begin(); note_book_it != note_book.end(); note_book_it++){

		book_list.push_back(note_book_it->first);
	}

	return book_list;
}


/**************************************************************************************
	@brief	:	Get book nots
**************************************************************************************/
Note_list Note_Manager::get_book_notes(string book)
{
	Note_list notes;	
	notes.clear();

	/* Find book */		
	if (note_book.find(book) != note_book.end()){

		notes = note_book[book];
	}

	return notes;
}

Note_list Note_Manager::get_book_notes(const char *book)
{
	return get_book_notes(string(book));
}


/*************************************************************************************
	@brief	:	Output a anchor 
*************************************************************************************/
inline void add_anchor(ostream &out, const string &context, const string &id, bool spc=false)
{
	/* Anchor */
	out << "<a id = \"" << id << "\">" << endl;
	out << context << endl;
	out << "</a>" << endl;

	/* Split line */
	if (spc){
		
		out << "---" << endl;
	}
}

/*************************************************************************************
	@brief	:	Output a anchor 
*************************************************************************************/
inline void add_int_jump(ostream &out, const string &context, const string &id, const string &pre="")
{
	/* Jump */	
	out << pre << " [" << context << "](#" << id << ")" << endl;
}


/*************************************************************************************
	@brief	:	Note_Manager overload output << operator
*************************************************************************************/
ostream &operator << (ostream& out, const Note_Manager &nm)
{
	/* Create a iterator point to first book */
	stringstream format;
	unsigned int book_idx;
	Note_list::const_iterator note_it;
	Note_book_list::const_iterator book_it;

	/* Out puts contents */
	add_anchor(out, "# 目录", "目录", true);	
	
	/* Output each books name and it nots number */
	for (book_idx = 1, book_it = nm.note_book.begin(); book_it != nm.note_book.end(); book_it++, book_idx++){

		format.str("");
		format << book_idx << ". " << book_it->first;
		add_int_jump(out, format.str(), book_it->first, "###");
	}

	/* First level, process book */
	for (book_it = nm.note_book.begin(); book_it != nm.note_book.end(); book_it++){

		/* Out put book name, with anchor*/
		add_anchor(out, "# " + book_it->first, book_it->first);

		/* Add a internal jump, back to contents */
		add_int_jump(out, "返回目录", "目录", "###");

		/* Second level process note */
		for (note_it = book_it->second.begin(); note_it != book_it->second.end(); note_it++){
		
			out << *note_it << endl;	
		}

	}
	
	return out;
}


/*************************************************************************************
	@brief	:	Note manager output markdown format file
*************************************************************************************/
bool Note_Manager::markdown_output(const string file_name)
{
	string md_extd(".md");
	string xml_extd(".xml");
	Note_book_list::iterator it;
	string output_name = file_name;

	/* Process filename */
	if (file_name.find(xml_extd) != string::npos){

		output_name.replace(file_name.find_first_of(xml_extd), xml_extd.size(), md_extd);
	}
	else{

		output_name += md_extd; 
	}

	/* Markdown format output */
	ofstream markdown(output_name.c_str(), ofstream::out);

	if (!markdown){

		cout << "Open markdown file error!" << endl;
		return false;
	}

	/* Sort note book note, make it inorder by page number */
	for (it = note_book.begin(); it != note_book.end(); it++){
		
		/* Sort notes, make it in order by page number */
		sort(it->second.begin(), it->second.end(), is_page_number_little);
	}

	/* Markdown output */
	markdown << *this << endl;

	return true;
}

