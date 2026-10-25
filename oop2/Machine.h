#ifndef MACHINE_H
#define MACHINE_H

#include <vector>
#include "Job.h"

using namespace std; 

class Machine{
	private:
		int machineId;
		int totalTime;
		int currentJobTime;
		bool isFree;
		
	public:
		Machine(){
			this->totalTime = 0;
			this->machineId = -1;
			this->isFree = true;
		}
		//setters - getters
		void setTotalTime(int time){
			this->totalTime = time;
		}
		
		int getTotalTime(){
			return this->totalTime;
		}
		
		
		
		void setMachineId(int i){
			this->machineId = i;
		}
		
		int getMachineId(){
			return this->machineId;
		}
		
		void setFree(){
			this->isFree = true;
		}
		
		void setOccupied(){
			this->isFree = false;
		}
		
		bool getFree(){
			return this->isFree;
		}
		
		void *description(char *ret){
			sprintf(ret,"Machine: %d Total Working Time: %d",this->machineId, this->totalTime);
		}
		
};

#endif
