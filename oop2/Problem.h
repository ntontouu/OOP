#include <fstream>
#include <vector>
#include <algorithm>
#include "Job.h"
#include "Machine.h"

using namespace std; 

class Problem{
	private:
		//the number of jobs to be executed
		int numberOfJobs;
		//the number of machines to be used
		int numberOfMachines;
		//the optimal time 
		int optimalEndTime;
		//a vector containing the time for each job to be executed to each machine
		vector<vector<int> > *jobs;
		//a vector containing the job execution sequence to eache machine
		vector<vector<Machine> > *visitSeq;
		//a vector containing all the jobs to be executed
		vector<Job *> *jobsSet;
		//a vector containing all machines to be used
		vector<Machine *> *machinesSet;
		int timer;
		
		
		vector<int> *jobSeq;
		
	public:
		Problem(char *filename){
			this->timer = 0;
			jobs = new vector<vector<int> >;
			ifstream infile(filename);
			//reads the number of jobs and creates a vector containing them
			infile >> numberOfJobs;
			jobsSet = new vector<Job *>();
			for (int i=0;i<numberOfJobs;i++){
				Job *j = new Job();
				j->setId(i+1);
				jobsSet->push_back(j);
			}
			//reads the number of machines and creates a vector containing them
			infile >> numberOfMachines;
			machinesSet = new vector<Machine *>();
			for (int i=0;i<numberOfMachines;i++){
				Machine *m = new Machine();
				m->setMachineId(i+1);
				machinesSet->push_back(m);
			}
			//reads the optimal ending time
			infile >> optimalEndTime;
			//reads jobs execution times to each machine
			//and updates corresponding object's status
			for (int i = 0;i<numberOfJobs; i++){
				vector<int> *v = new vector<int>();
				for (int j = 0;j<numberOfMachines;j++){
					int x;
					infile >> x;
					v->push_back(x);
				}
				jobsSet->at(i)->setTimeToMachines(v);
			}
			//reads jobs execution sequence to each machine
			//and updates corresponding object's status
			visitSeq = new vector<vector<Machine> >;
			for (int i = 0;i<numberOfJobs; i++){
				vector<Machine *> *v = new vector<Machine *>();
				for (int j = 0;j<numberOfMachines;j++){
					int x;
					infile >> x;
					v->push_back(machinesSet->at(x-1));
				}
				jobsSet->at(i)->setMachinesSeq(v);
			}
			//updating desired execution time for each job 
			for (int i = 0;i<numberOfJobs;i++){
				double sum = 0;
				for (int j=0;j<numberOfMachines;j++){
					Job *_j = jobsSet->at(i);
					int x = _j->getTimeToMachines()->at(j);
					sum+=(double)x;
				}
				(jobsSet->at(i))->setDesiredTime((int)(1.3*sum));
				
			}
			
			jobSeq = new vector<int>();
		}
		
		int getTimer(){
			return this->timer;
		}
		
		vector<Machine *> *getMachines(){
			return this->machinesSet;
		}
		
		vector<Job *> *getJobs(){
			return this->jobsSet;
		}
		//check if all jobs status is END -> THE END OF PROCESS
		bool end(){
			bool e = true;
			for (int i = 0; i<this->jobsSet->size();i++){ 
				if (this->jobsSet->at(i)->getStatus()!=END){
					e = false;	
				}
				else if (this->jobsSet->at(i)->getAllTime() == 0){
					this->jobsSet->at(i)->setAllTime(timer);
				}
			}
			return e;
		}
		/**
		Each job is placed to the machine defined by the sequence given.
		The process ends when all jobs have reached their desired time
		**/
		void solution(){
			//each job initially is to be executed and waiting for a free machine
			for (int i = 0; i<this->jobsSet->size();i++){
				this->jobsSet->at(i)->setStatus(WAIT);
			}
			//for each time unit
			while (true){
				//each job tries to reserve a machine
				for (int i = 0; i<this->jobsSet->size();i++){
					this->jobsSet->at(i)->execution(1, timer, jobSeq);
				}
				timer++;
				if (this->end()){
					cout << "Total Execution time:" << timer << endl;
					break;
				}
			}
		}
		bool compareJobsByTotalTime(Job *j1, Job *j2){
			return (j1->workingTime()<j2->workingTime());
		}
		bool compareJobsByDesiredTime(Job *j1, Job *j2){
			return (j1->getDesiredTime()<j2->getDesiredTime());
		}
		//bubble sort ordering using compareJobsByTotalTime
		void sortJobsByTotalTime(){
			bool isUnsorted; 
			int n = this->jobsSet->size();
		    do { 
		        isUnsorted = false; 
		        for (int i = 0; i < n-1; i++) { 
		            if (compareJobsByTotalTime(this->jobsSet->at(i) , this->jobsSet->at(i+1))) { 
		                isUnsorted = true; 
		                for (; i < (n - 1); i++) { 
		                    if (compareJobsByTotalTime(this->jobsSet->at(i) , this->jobsSet->at(i+1))) { 
		                        std::swap(this->jobsSet->at(i), this->jobsSet->at(i+1)); 
		                    } 
		                } 
		            } 
		        } 
		    } while (isUnsorted); 
			
		}
		//bubble sort ordering using compareJobsByDesiredTime
		void sortJobsByDesiredTime(){
			bool isUnsorted; 
			int n = this->jobsSet->size();
		    do { 
		        isUnsorted = false; 
		        
		        for (int i = 0; i < n-1; i++) { 
		            if (compareJobsByDesiredTime(this->jobsSet->at(i) , this->jobsSet->at(i+1))) { 
		                isUnsorted = true; 
		                for (; i < (n - 1); i++) { 
		                    if (compareJobsByDesiredTime(this->jobsSet->at(i) , this->jobsSet->at(i+1))) { 
		                        std::swap(this->jobsSet->at(i), this->jobsSet->at(i+1)); 
		                    } 
		                } 
		            } 
		        } 
		    } while (isUnsorted); 
		}
		
		bool compareMachineTimes(Machine *m1, Machine *m2){
			return (m1->getTotalTime() < m2->getTotalTime());
		}
		//compare machines by compareMachineTimes
		void sortMachinesByWorkingTime(){
			bool isUnsorted; 
			int n = this->machinesSet->size();
		    do { 
		        isUnsorted = false; 
		        
		        for (int i = 0; i < n-1; i++) { 
		            if (compareMachineTimes(this->machinesSet->at(i) , this->machinesSet->at(i+1))) { 
		                isUnsorted = true; 
		                for (; i < (n - 1); i++) { 
		                    if (compareMachineTimes(this->machinesSet->at(i) , this->machinesSet->at(i+1))) { 
		                        std::swap(this->machinesSet->at(i), this->machinesSet->at(i+1)); 
		                    } 
		                } 
		            } 
		        } 
		    } while (isUnsorted);
		}
		//reads jobSed tupples of size 3 and makes the graph
		void printGraph(){
			char G[5][timer];
			for (int i = 0; i< 5; i++){
				for (int j=0; j<timer;j++){
					G[i][j]='#';
				}
			}
			for (int i=0;i<this->jobSeq->size();i+=3){
				int m = (this->jobSeq->at(i+1))-1;
				int t = (this->jobSeq->at(i));
				char job = (this->jobSeq->at(i+2))+'0';
				cout << m <<  " " << job << " " << t << endl;
				if (((m>=0) && (m<5)) && ((t>=0) && (t<timer)) && ((job>'0') && (job<='5'))){
					G[m][t]=job;
				}
				
			}
			
				for (int j=0; j<timer;j++){
					for (int i = 0; i< 5; i++){
					cout << G[i][j];
				}
				cout << endl;
			}	
			
		}
};
