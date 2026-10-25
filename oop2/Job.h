#ifndef JOB_H
#define JOB_H

#include <vector>
#include "Machine.h"

#define WAIT 0
#define EXE 1
#define END -1

using namespace std; 

class Job{
	private:
		int id;
		//the time that the job have been executed
		int totalTime;
		//the total time from the begining till the end of job
		int allTime;
		//the desired time for that job to be executed
		int desiredTime;
		//it contains the time that the job can be assigned to each machine
		vector<int> *timeToMachines;
		//it defines the sequences of machines that the job can reserve
		vector<Machine *> *machinesSeq;
		//the machine that is currently reserved
		int currentMachine;
		//the job's current status
		int timeInCurrentMachine;
		//the job's current status
		int status;
		
	public:
		Job(){
			this->id = 0;
			this->totalTime = 0;
			this->desiredTime = 0;
			this->timeToMachines = new vector<int>();
			this->machinesSeq = new vector<Machine *>();
			this->currentMachine = 0;
			this->status = WAIT;
			this->allTime = 0;
		}
		//setters - getters
		int getId(){
			return this->id;
		}
		
		void setId(int id){
			this->id = id;
		}
		
		void setMachinesSeq(vector<Machine *> *m){
			this->machinesSeq = m;
		}
		
		void setTimeToMachines(vector<int> *t){
			this->timeToMachines = t;
		}
		
		vector<Machine *> *getMachinesSeq(){
			return this->machinesSeq;
		}
		
		vector<int> *getTimeToMachines(){
			return this->timeToMachines;
		}
		
		void setTotalTime(int time){
			this->totalTime = time;
		}
		
		int getTotalTime(){
			return this->totalTime;
		}
		//se the desired time
		void setDesiredTime(int time){
			this->desiredTime = time;
		}
		//get the desired time
		int getDesiredTime(){
			return this->desiredTime;
		}
		
		int getStatus(){
			return this->status;
		}
		
		int setStatus(int s){
			this->status = s;
		}
		//set the overall time in system
		void setAllTime(int time){
			this->allTime = time;
		}
		//get the overall time in system
		int getAllTime(){
			return this->allTime;
		}
		//returns the job total time (for all machines)
		int workingTime(){
			int sum = 0;
			int i;
			for (i=0;i<this->timeToMachines->size();i++){
				sum+=this->timeToMachines->at(i);
			}
			return sum;
		}
		
		int execution(int t, int timer, vector<int> *jobs){
			//cout << this->id << " " << this->machinesSeq->at(this->currentMachine)->getMachineId() << " " << this->totalTime << " " << endl;
			//the job is executing and has to be continued
			if ((this->status == EXE) && (this->totalTime < this->timeInCurrentMachine) ){
				this->totalTime+=t;
				//add the tupple containing time - job - machine
				int v[3];
				v[0]=timer;
				v[1]=(this->machinesSeq->at(this->currentMachine))->getMachineId();
				v[2]=this->id;
				
				jobs->push_back(v[0]);
				jobs->push_back(v[1]);
				jobs->push_back(v[2]);
				return t;
			}
			//after reserving the machine for t time units the job has not time to be executed
			else if ((this->status == EXE) && (this->totalTime == this->timeInCurrentMachine)){
				this->machinesSeq->at(this->currentMachine)->setTotalTime(this->machinesSeq->at(this->currentMachine)->getTotalTime()+this->timeInCurrentMachine);
				this->machinesSeq->at(this->currentMachine)->setFree();
				if (this->currentMachine==4){
					this->status = END;
				}
				else {
					this->status = WAIT;
					this->currentMachine=this->currentMachine+1;
					this->totalTime=0;
				}
				this->machinesSeq->at(this->currentMachine)->setFree();
				return t;
			}
			//the job is waiting and the desired machine is free
			else if ((this->status == WAIT) && this->machinesSeq->at(this->currentMachine)->getFree()){
				this->status = EXE;
				this->timeInCurrentMachine = this->getTimeToMachines()->at(this->machinesSeq->at(this->currentMachine)->getMachineId() - 1);
				cout << "Time:" << timer << " -> Job " << this->getId() << " entered machine " << this->machinesSeq->at(this->currentMachine)->getMachineId() << " for " << this->timeInCurrentMachine << " timeUnits" << endl;
				(this->machinesSeq->at(this->currentMachine))->setOccupied();
				this->totalTime+=t;
				//add the tupple containing time - job - machine
				int v[3];
				v[0]=timer;
				v[1]=(this->machinesSeq->at(this->currentMachine))->getMachineId();
				v[2]=this->id;
				
				jobs->push_back(v[0]);
				jobs->push_back(v[1]);
				jobs->push_back(v[2]);
				return t;
			}
			//the job is waiting and the desired machine is not free
			else if ((this->status == WAIT) && !this->machinesSeq->at(this->currentMachine)->getFree()){
				return 0;
			}
			else {
				return 0;
			}
		}
		//return a string containing information about the job
		void *description(char *ret){
			//create returning string
			sprintf(ret,"Job: %d Estimated Working Time: %d Total time:%d Working Time: %d\n",this->id,this->desiredTime,this->allTime, this->workingTime() );
			sprintf(ret,"%sMachine Sequence\n", ret);
			int i;
			//time to each machine
			for (i=0;i<this->machinesSeq->size();i++){
				sprintf(ret,"%sMachine: %d Time: %d\n",ret,this->machinesSeq->at(i)->getMachineId(),this->timeToMachines->at(this->machinesSeq->at(i)->getMachineId()-1));
			}
			return ret;
		}		
};

#endif

