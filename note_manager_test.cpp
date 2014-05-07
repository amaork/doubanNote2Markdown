#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "note_manager.h"

int main(int argc, char **argv)
{
	int i;
	Note_Manager *nm = new Note_Manager;

	for (i = 0; i < 10; i++){

		nm->add_note("Test", Note("1234567", "https://github.com/amaork/douban_read_notes_to_markdown", i*128));
	}

	cout << *nm << endl;

	delete nm;
	return 0;
}

