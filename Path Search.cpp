#include<iostream>
#include<list>
#include<stack>
#include<fstream>
using namespace std;

struct data {
	int index;
	string name;
	int count_out;
	int* out;
	bool seen = false;
	int predecessor = -1;
};




class Path {
	list<struct data>list_1;
public:

	void read(ifstream& iff) {

		
		while (!iff.eof()) {


			

			struct data temporary;
			iff >> temporary.index;
			char temp;
			temporary.name = "";

			iff >> temp;
			while (temp != ',') {
				temporary.name += temp;
				iff >> temp;
			}

			iff >> temporary.count_out;

			temporary.out = new int[temporary.count_out];


			for (int i = 0;i < temporary.count_out;i++)
				iff >> temporary.out[i];

			list_1.push_back(temporary);
		}
	}

	void show() {
		list<struct data>::iterator it;
		it = list_1.begin();
		while (it != list_1.end()) {
			cout << it->index << "  " << it->name << "   " << it->count_out << "  ";
			for (int i = 0;i < it->count_out;i++)
				cout << it->out[i] << " ";
			cout << "seen = " << it->seen << "  pred= " << it->predecessor;
			cout << endl;

			it++;
		}
	}

	struct data* get_location() {
		struct data* location;
		string name;
		cout << "enter starting location= ";
		cin >> name;
		do {
			location = find_loc(name);

			if (!validate(location)) {
				cout << "invalid location.....\nplease reenter = ";
				cin >> name;
			}

		} while (!validate(location));

		return location;
	}

	struct data* find_loc( string name_loc) {

		list<struct data>::iterator it;
		it = list_1.begin();
		struct data* temp = new struct data;

		while (it != list_1.end()) {

			if (it->name == name_loc) {
				*temp = *it;
				return temp;
			}
			it++;
		}

		return NULL;
	}


	bool validate(struct data* temp) {
		return  (temp == NULL ? false : true);
	}

	void search(struct data* start, struct data* end) {
		char choice;
		cout << "Want to apply back track (press b) search or recursive ( press r )\n ";
		cin >> choice;
		if (choice == 'b') {
			backtrack_search(*start, *end);
			path(end);
		}
		else if (choice == 'r') {
			recursive_search(start, end);
			path(end);
		}
		else
			cout << "Invalid choice\n";
	}

	void set_seen_true(struct data temp) {
		list<struct data>::iterator it;
		it = list_1.begin();

		while (it != list_1.end()) {

			if (it->name == temp.name && it->index == temp.index) {
				it->seen = true;
				return;
			}
			it++;
		}
	}

	void recursive_search( struct data* current, struct data* destination) {
		if (current == destination) {
			return;
		}


		for (int i = 0;i < current->count_out;i++) {

			list<struct data>::iterator it;
			it = list_1.begin();

			for (int j = 0;j < current->out[i];j++)
			{
				it++;
			}

			if (!is_seen(*it)){

				//set_seen_true(*it);
				it->seen = true;
				it->predecessor = current->index;

				struct data* temporary = new struct data;
				*temporary = *it;
				recursive_search(temporary, destination);
				delete temporary;

			}

		}

	}

	bool is_seen(struct data temp) {
		return (temp.seen == true);
	}

	void path(struct data* destination) {
		find_path(find_loc(destination->name));
	}

	void find_path( struct data* destination) {
		if (destination->predecessor == -1) {
			cout << destination->name << " -> ";
			return;
		}

		list<struct data>::iterator it;
		it = list_1.begin();
		for (int j = 0;j < (destination->predecessor);j++)
		{
			it++;
		}

		struct data* temporary = new struct data;
		*temporary = *it;

		find_path(temporary);
		cout << destination->name << " -> ";
	}

	void backtrack_search(struct data loc, struct data end_pos) {
		stack<struct data> st;
		st.push(loc);

		while (!st.empty()) {

			loc = st.top();
			st.pop();

			if (loc.name == end_pos.name && loc.index == end_pos.index) {
				//cout << "ahhhh we found if finally \n";
				return;
			}

			for (int i = 0;i < loc.count_out;i++) {

				list<struct data>::iterator it;
				it = list_1.begin();

				for (int j = 0;j < loc.out[i];j++)
				{
					it++;
				}

				if (it->seen == false) {
					it->seen = true;
					it->predecessor = loc.index;

					st.push(*it);
				}
			}
		}
	}

};
int main() {

	ifstream iff("first.txt");
	if (!iff)
		cout << "can't open the file\n";
	int size;
	iff >> size;
	cout << size;
     
	Path p1;
	p1.read(iff);

	p1.show();
	///////////////////////////////////////////////////////////////////////////////



	string name_start, name_end;
	struct data* intial_loc = new struct data;
	cout << "=========  Staring City  ==================\n";
	intial_loc = p1.get_location();

	struct data* end_loc = new struct data;
	cout << "=========  Destination City  ==================\n";
	end_loc = p1.get_location();

	
	p1.set_seen_true(*intial_loc);

	
	p1.search(intial_loc, end_loc);      
	return 0;
}