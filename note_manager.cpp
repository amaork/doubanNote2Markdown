#include <algorithm>
#include <fstream>
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
	/* Write page and url */
	out << "## [P" << note.page << "](" << note.url << ")" << endl;
			
	/* Write context */	
	if (note.context.size()){

		out << note.context << endl << endl;
	}

	/* Write remark */
	if (note.remark.size()){

		out << "<font color=#7f7f7f>" << endl;
		out	<< note.remark << endl;
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
	@brief	:	Note_Manager overload output << operator
*************************************************************************************/
ostream &operator << (ostream& out, const Note_Manager &nm)
{
	/* Create a iterator point to first book */
	Note_list::const_iterator note_it;
	Note_book_list::const_iterator book_it;

	/* First level, process book */
	for (book_it = nm.note_book.begin(); book_it != nm.note_book.end(); book_it++){

		/* Out put book name*/
		out << "# " << book_it->first << endl;

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
	Note_book_list::iterator it;

	/* Markdown format output */
	ofstream markdown(file_name.c_str(), ofstream::out);

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

