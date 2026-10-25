#include <stdlib.h>
#include <iostream>
#include "Problem.h"

using namespace std;

int menu(){
	int c;
	cout << endl;
	cout << "Choose Job Number (1 up to 10)" << endl;
	cout << "11. Sort jobs by total execution time" << endl;
	cout << "12. Sort jobs by desired time" << endl;
	cout << "13. Sort machines by working time" << endl;
	cout << "14. Show solution graph" << endl;
	cin >> c;
	return c;
}



/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	int j;
	char *c = (char *)malloc(1024);
	if (argc<2){
		cout << "File name not given!" << endl;
		return 0;
	}
	sprintf(c,"%s",argv[1]);
	Problem *p = new Problem(c);
	p->solution();
	cout << "Total execution time: "<< p->getTimer() << endl << endl;
	int i = menu();
	char *response = (char *)malloc(1000);
	while (i!=0){
		if ((i>=1) && (i<=10)){
			(p->getJobs()->at(i-1))->description(response);
			cout << response << endl;
		}
		else if (i==11){
			p->sortJobsByTotalTime();
			for (int j=0;j<10;j++){
				(p->getJobs()->at(j))->description(response);
				cout << response << endl << endl;
			}
		}
		else if (i==12){
			p->sortJobsByDesiredTime();
			for (int j=0;j<10;j++){
				(p->getJobs()->at(j))->description(response);
				cout << response << endl << endl;
			}
		}
		else if (i==13){
			p->sortMachinesByWorkingTime();
			for (int j=0;j<5;j++){
				(p->getMachines()->at(j))->description(response);
				cout << response << endl << endl;
			}
		}
		else if (i==14){
			cout << endl << endl;
			p->printGraph();
			cout << endl << endl;
		}
		i = menu();
	}
	
	return 0;
}

