#include "Pomodoro.hpp"
	
	int Pomodoro::counter = 0;
	
	int Pomodoro::totalTime = 0;
	
	//Default constructor
	Pomodoro::Pomodoro(){
		//25 minutes
		this->workDuration = 25*60*CLOCKS_PER_SEC;
		//5 minutes
		this->breakDuration = 5*60*CLOCKS_PER_SEC;
		//time elapsed initially : 0
		this->workTimeElapsed = 0;
		this->breakTimeElapsed=0;
		//initial status : 0
		this->status = STATUS_HOLD;
	}
	//constructor 
	Pomodoro::Pomodoro(int wd, int bd){
		//time in minutes
		this->workDuration = wd*60*CLOCKS_PER_SEC;
		this->breakDuration = bd*60*CLOCKS_PER_SEC;
		this->status = STATUS_HOLD;
		this->workTimeElapsed = 0;
		this->breakTimeElapsed=0;
	}
	
	Pomodoro::~Pomodoro(){
		if (this->status != STATUS_HOLD && this->status != STATUS_END){
			this->endSession();
		}
		cout << "Pomodoro Destroyed" << endl; 
	}
	
	//ΑΡΧΗ ΤΗΣ ΣΥΝΟΔΟΥ
	void Pomodoro::startSession(){
		//ΗΧΗΤΙΚΟ ΣΗΜΑ
		beep();
		sleep(0.9);
		beep();
		sleep(0.6);
		beep();
		sleep(0.3);
		beep();
		//ΚΡΑΤΑΜΕ ΤΟΝ ΧΡΟΝΟ ΠΟΥ ΞΕΚΙΝΗΣΕ
		this->chronoStarted = clock();
		//Η ΚΑΤΑΣΤΑΣΗ ΑΛΛΑΖΕΙ
		this->status = STATUS_STARTED;
		//ΜΗΔΕΝΙΖΟΝΤΑΙ ΟΙ ΧΡΟΝΟΙ 
		this->workTimeElapsed = 0;
		this->breakTimeElapsed=0;
	}
	
	int Pomodoro::run(){
		int t;
		//ΕΛΕΓΧΟΥΜΕ ΣΕ ΤΙ ΚΑΤΑΣΤΑΣΗ ΕΙΝΑΙ Ο POMODORO
		//AN EINAI ΣΕ ΛΕΙΤΟΥΡΓΙΑ
		if ((this->status==STATUS_STARTED) || (this->status==STATUS_RESUMED)){
			t = clock() - this->chronoStarted;
			//ΚΡΑΤΑΜΕ ΤΗΝ ΤΡΕΧΟΥΣΑ ΧΡΟΝΙΚΗ ΣΤΙΓΜΗ
			this->chronoStarted = clock();
			//ΥΠΟΛΟΓΙΖΟΥΜΕ ΤΟ ΧΡΟΝΟ ΠΟΥ ΠΕΡΑΣΕ
			this->workTimeElapsed+=t;
			//ΥΠΟΛΟΓΙΖΟΥΜΕ ΤΟ ΣΥΝΟΛΙΚΟ ΧΡΟΝΟ
			totalTime+=t;
			//ΑΝ ΕΧΟΥΜΕ ΞΟΔΕΨΕΙ ΟΛΟ ΤΟ ΧΡΟΝΟ ΓΙΑ ΕΡΓΑΣΙΑ
			if (this->workTimeElapsed >= this->workDuration){
				//ΟΛΟΚΛΗΡΩΝΕΤΑΙ ΤΟ SESSION
				this->endSession();
				return 0;
			}
		}
		//ΑΝ ΕΧΕΙ  ΔΙΑΚΟΠΕΙ
		else if (this->status == STATUS_PAUSED){
			t = clock() - this->chronoEnd;
			this->chronoEnd = clock();
			this->breakTimeElapsed+=t;
			//ΑΝ ΕΧΟΥΜΕ ΞΟΔΕΨΕΙ ΟΛΟ ΤΟ ΧΡΟΝΟ ΓΙΑ ΕΡΓΑΣΙΑ
			if (this->breakTimeElapsed >= this->breakDuration){
				this->resume();
				return 1;
			}
		}
		else if (this->status == STATUS_END){
			return 0;
		}
		return 1;
	}
	
	
	
	
	//ΤΕΛΟΣ ΤΗΣ ΣΥΝΟΔΟΥ
	void Pomodoro::endSession(){
		//ΚΡΑΤΑΜΕ ΤΟ ΧΡΟΝΟ ΟΛΟΚΛΗΡΩΣΗΣ
		this->chronoEnd = clock();
		//Η ΣΥΝΟΔΟΣ ΜΠΑΙΝΕΙ ΣΕ ΚΑΤΑΣΤΑΣΗ END
		this->status = STATUS_END;
		//ΑΥΞΑΝΕΤΑΙ Ο ΑΡΙΘΜΟΣ ΤΩΝ ΟΛΟΚΛΗΡΩΜΕΝΩΝ SESSION
		Pomodoro::counter++;
		//ΗΧΗΤΙΚΟ ΣΗΜΑ
		beep();
		sleep(0.9);
		beep();
		sleep(0.6);
		beep();
		sleep(0.3);
		beep();
		cout << "\a";
	}
	
	//Η ΣΥΝΟΔΟΣ ΠΑΓΩΝΕΙ
	void Pomodoro::pause(){
		//Ο ΧΡΟΝΟΣ ΠΟΥ ΠΑΓΩΝΕΙ Η ΣΥΝΟΔΟΣ
		this->chronoEnd = clock();
		this->status = STATUS_PAUSED;
	}
	
	void Pomodoro::resume(){
		//Ο ΧΡΟΝΟΣ ΠΟΥ ΞΑΝΑΞΕΚΙΝΑΕΙ Η ΣΥΝΟΔΟΣ
		this->chronoStarted = clock();
		this->status = STATUS_RESUMED;
	}
	//ΕΠΙΣΤΡΕΦΕΙ ΜΗΝΥΜΑ ΜΕ ΤΟ ΧΡΟΝΟ ΛΕΙΤΟΥΡΓΙΑΣ ΚΑΙ ΔΙΑΛΕΙΜΜΑΤΟΣ ΠΟΥ ΕΧΕΙ ΠΕΡΑΣΕΙ
	char *Pomodoro::showTime(){
		char *timeString = (char *)malloc(1024);
		sprintf(timeString,"Working Time:%ld Break Time Elapsed: %ld",(this->workTimeElapsed/CLOCKS_PER_SEC),(this->breakTimeElapsed/CLOCKS_PER_SEC));
		return timeString;
	}
	//ΣΕ mseconds
	void Pomodoro::setWorkDuration(int wd){
		this->workDuration = wd*60*CLOCKS_PER_SEC;
	}
	//ΣΕ mseconds
	void Pomodoro::setBreakDuration(int bd){
		this->breakDuration = bd*60*CLOCKS_PER_SEC;
	}
	//ΣΕ seconds
	int Pomodoro::getWorkDuration(){
		return this->workDuration/CLOCKS_PER_SEC;
	}
	//ΣΕ seconds
	int Pomodoro::getBreakDuration(){
		return this->breakDuration/CLOCKS_PER_SEC;
	}
	//SETTERS & GETTERS
	void Pomodoro::setChronoStarted(int x){
		this->chronoStarted = x;
	}
	
	int Pomodoro::getChronoStarted(){
		return this->chronoStarted;
	}
	
	void Pomodoro::setChronoEnd(int x){
		this->chronoEnd = x;
	}
	
	int Pomodoro::getChronoEnd(){
		return this->chronoEnd;
	}
	
	void Pomodoro::setStatus(int x){
		this->status = x;
	}
	
	int Pomodoro::getStatus(){
		return this->status;
	}
	//seconds
	int Pomodoro::getWorkTimeElapsed(){
		return this->workTimeElapsed/CLOCKS_PER_SEC;
	}
	//seconds
	void Pomodoro::setWorkTimeElapsed(int x){
		this->workTimeElapsed = x*CLOCKS_PER_SEC;
	}
	//seconds
	int Pomodoro::getBreakTimeElapsed(){
		return this->breakTimeElapsed/CLOCKS_PER_SEC;
	}
	//seconds
	void Pomodoro::setBreakTimeElapsed(int x){
		this->workTimeElapsed = x*CLOCKS_PER_SEC;
	}
	
	void Pomodoro::setId(int x){
		this->id = x;
	}
		
	int Pomodoro::getId(){
		return this->id;
	}
	
	int Pomodoro::getCounter(){
		return Pomodoro::counter;
	}
	//seconds
	int Pomodoro::getTotalTime(){
		return Pomodoro::totalTime/CLOCKS_PER_SEC;
	}
	
	void Pomodoro::setCounter(){
		Pomodoro::counter = 0;
	}
	
	
	
	void Pomodoro::setTotalTime(){
		Pomodoro::totalTime = 0;
	}
