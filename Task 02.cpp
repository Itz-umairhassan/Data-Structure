#include<iostream>
#include<list>
#include<fstream>
#include<vector>
#include<queue>
#include<string>
using namespace std;

#define SIMULATION_TIME 900

class Time_calculation {
	static float waiting, turnaround;
	static int w, t;
public:
	
	void insert_wait_turn(int waiting, int turnaround) {
		this->waiting += float(waiting);
		this->turnaround += float(turnaround);
		w += 1;
		t += 1;

		//
	}

	void show_averageTimes() {
		//cout << this->w << "   " << this->t << endl;
		float wait = this->waiting / w;
		float turn = this->turnaround / t;
		
		cout << "Average Waiting time = " << wait << endl;
		cout << "Average Turn Around time = " << turn << endl;
	}
};

int Time_calculation::t = 0;int Time_calculation::w = 0;
float Time_calculation::turnaround = 0;float Time_calculation::waiting = 0;

///////////////////////////////////////////////////////////////////////////////


struct task {
	string name;
	int time;
	int arrival_time;
	task() {
		name = "";
		time = -1;
		arrival_time = -1;
	}

	task(string n,int t,int arr) {
		name = n;
		time = t;
		arrival_time = arr;
	}
};

void display(task t) {
	cout << t.name << t.time << endl;
}

bool compare_time(task t1, task t2) {
	return t1.time < t2.time;
}

///////////////////////////////////////////////////////////////////////////

class scheduling {
	list<task>l;
	queue<task>q;
	task prev_top;
public:

	void write() {

		char respones;
		cout << "Want to enter some data press y = ";
		cin >> respones;
		ofstream off("readylist.txt", ios::trunc);
		if (!off)
			cout << "Can't open file\n";
		int time;
		string name;

		while (respones == 'Y' || toupper(respones) == 'Y') {
			cout << "Enter name = ";
			cin >> name;
			cout << "Enter Execution time = ";
			cin >> time;
			off << '\n';
			off << name;
			off << " ";
			off << time;

			cout << "Want to enter more data press y = ";
			cin >> respones;
		}

		off.close();
	}


	void read(int tick) {
		ifstream iff("readylist.txt");
		if (!iff)
			cout << "File not open\n";
		if (iff.peek() == iff.eof())
			return;
		while (!iff.eof()) {
			int time, arrival;
			string name;
			iff >> name;
			iff >> time;

			task temp(name, time, tick);
			 
			l.push_back(temp);
		}
		
		iff.close();

		ofstream off("readylist.txt", ios::trunc);
		off.close();
	}

	void display2() {
		list<task>::iterator it;
		it = l.begin();
		while (it != l.end()) {
			cout << it->name << " " << it->time << "  " << it->arrival_time << endl;
			it++;
		}
	}

	void sort() {
		vector<task>v;
		while (!l.empty()) {
			task temp = l.front();
			l.pop_front();
			v.push_back(temp);
		}
		
		for (int i = 0; i < v.size(); i++) {

			for (int j = 0; j < v.size()  - 1; j++) {
				
				if (v[j].time < v[i].time) {
					swap(v[j], v[i]);
					
				}
			}
		}
		
		l.clear();
		int m = 0;
		while (m<v.size()) {
			l.push_back(v[m]);
			m++;
		}

		v.clear();
	}


	void start_req(int time, int& seq);
	//void requests(int arrival_time);
	int assign_request(int time);

	void set_top(task temp) {  
		prev_top = temp;
	}
	// later we'll use it in main function for waiting and turnaround time...
	task get_top() {
		return prev_top;
	}

	//void remove_top() { q.pop(); }
};

void scheduling::start_req(int time, int& seq) {
	//while (!q.empty()) { q.pop(); }
	bool was_emp = q.empty();
	while (!q.empty()) {
		l.push_back(q.front());
		seq--;
		q.pop();
	}

	if (!was_emp)
		sort();

	while (!l.empty()) {
		task temp = l.front();
		q.push(temp);
		l.pop_front();

		cout << seq << " . " << " request with name " << temp.name << " having time " << temp.time << " at "
			<< time << "(Arrival Time = " << temp.arrival_time << " ) " << endl;
		seq++;
	}
}

int scheduling::assign_request(int time) {
	if (q.empty()) {
		cout << "Ready Queue is empty .... no more job assignment \n";

		Time_calculation t1;
		t1.show_averageTimes();
		
		exit(0);
	}
	
	task temp = q.front();
	
	cout << " Request " << temp.name << " having time " << temp.time << " is assigned at " << time <<
		endl << endl;
	set_top(temp);
	q.pop();
	return temp.time-1;
}

//////////////////////////////////////////////////////////////////////////////



int main() {
	scheduling s;
	Time_calculation t;

	/*s.read(2);
	s.display2();
	cout << endl << endl;
	s.write();
	s.read(3);
	s.display2();*/

	//s.sort();
	
	int sq = 1;
	int tick = 0, busyfor, exec_start = 0;
	s.read(tick);
	//s.sort();
	s.display2();

	cout << "\n\n";
	bool free = true;
	s.start_req(tick, sq);
	
	cout << "\n\n";
	while(tick<SIMULATION_TIME) {


		if (tick % 15 == 0) {
			//s.write();
			
			system("pause");
			s.read(tick);
			//s.sort();
			s.start_req(tick,sq);
			s.display2();
		}

		if (free == true) {
			busyfor = s.assign_request(tick);
			exec_start = tick;
			free = false;
		}
		else if (free == false) {
			if (busyfor > 0)
				busyfor -= 1;
			else {
				free = true;
				task temp = s.get_top();

				int waiting_time = exec_start- temp.arrival_time;
				int turn_around_time = tick - temp.arrival_time;

				t.insert_wait_turn(waiting_time, turn_around_time);

				cout << temp.name << " " << temp.time << " " << waiting_time 
					<< " " << turn_around_time << endl;
				cout << "CPU becomes free at " << tick << "\n\n";
				
				continue;
			}
		}

		tick++;
	}

	t.show_averageTimes();
	return 0;
}