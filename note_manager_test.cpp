#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "note_manager.h"

int main(int argc, char **argv)
{

	/* Note manager test */
	int i;
	Note_Manager *nm = new Note_Manager;

	for (i = 0; i < 10; i++){

		nm->add_note("Test", Note("1234567", "https://github.com/amaork/douban_read_notes_to_markdown", i*128));
	}

	cout << *nm << endl;

	delete nm;

	/* Markdown output test */
	string test = 	"http://img3.douban.com/view/page_note/large/public/p28269709-1.jpg我是测试的2http://img3.douban.com/view/page_note/large/public/p28269709-1.jpg12324245454http://img3.douban.com/view/page_note/large/public/p28269709-1.jpgADADADADAhttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpgDADADADADADADADADADA";

	string test2 = 	"http://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpg";

	string test3 = 	"AAAAAAAAAAAAAADADADAhttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpg";
	
	string test4 = 	"http://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jpghttp://img3.douban.com/view/page_note/large/public/p28269709-1.jp";

	markdown_output(cout, test, "http://", ".jpg");	
	markdown_output(cout, test2, "http://", ".jpg");	
	markdown_output(cout, test3, "http://", ".jpg");	
	markdown_output(cout, test4, "http://", ".jpg");	

	return 0;
}

