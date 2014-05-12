#ifndef	_NOTE_MANAGE_H_
#define _NOTE_MANAGE_H_

#include <map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/* Note */
class Note{

	public:

		/* Constructor */
		Note(const char *note, const char *url, unsigned int page);
	
		string context;
		string remark;
		string url;
		unsigned int page;
};

std::ostream& operator<<(std::ostream&, const Note& note);


/* Typedef */
typedef vector< Note> 	Note_list;
typedef vector<string>	Book_list;
typedef map<string, vector< Note> > Note_book_list;

/* Note manager */
class Note_Manager{

	public:

		/* Add a note with book name */
		void add_note(const char *book_name, Note note);
		void add_note(const char *book_name, Note &note);
		void add_note(const char *book_name, Note *note);
		void add_note(const string &book_name, Note &note);

		/* Get all books name */
		Book_list get_book_list(void);

		/* Get books note */
		Note_list get_book_notes(const char *book);
		Note_list get_book_notes(const string book);

		/* Markdown output */
		bool markdown_output(const string file_name="note.md");

		/* Book notes saved at here */
		Note_book_list note_book;

};

std::ostream& operator<<(std::ostream&, const Note_Manager& nm);

/* Output markdown centext with detect image url */
void markdown_output(ostream& out, const string &txt, const string &skey, const string &ekey);

#endif
