#ifndef Pomodoro_H
#define Pomodoro_H

#include <time.h>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>

#define STATUS_HOLD 0
#define STATUS_STARTED 1
#define STATUS_PAUSED 2
#define STATUS_RESUMED 3
#define STATUS_END 4

using namespace std;

class Pomodoro{
	private:
		//ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΣ ΕΡΓΑΣΙΑΣ
		int workDuration ; //25
		//ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΣ ΔΙΑΛΕΙΜΜΑΤΟΣ
		int breakDuration;  //5
		//ΧΡΟΝΟΣ ΕΡΓΑΣΙΑΣ ΠΟΥ ΕΧΕΙ ΚΑΤΑΝΑΛΩΘΕΙ
		int workTimeElapsed;
		//ΧΡΟΝΟΣ ΔΙΑΛΕΙΜΜΑΤΟΣ ΠΟΥ ΕΧΕΙ ΚΑΤΑΝΑΛΩΘΕΙ
		int breakTimeElapsed;
		//ΧΡΟΝΙΚΗ ΣΤΙΓΜΗ ΕΝΑΡΞΗΣ ΧΡΟΝΟΜΕΤΡΗΣΗΣ
		int chronoStarted;
		//ΧΡΟΝΙΚΗ ΣΤΙΓΜΗ ΛΗΞΗΣ ΧΡΟΝΟΜΕΤΡΗΣΗΣ
		int chronoEnd;
		//Η ΚΑΤΑΣΤΑΣΗ ΣΤΗΝ ΟΠΟΙΑ ΒΡΙΣΚΕΤΑΙ ΤΟ POMODORO
		int status;
		//TO ΜΟΝΑΔΙΚΟ ΑΝΑΓΩΝΡΙΣΤΙΚΟ ΤΟΥ POMODORO
		int id;
		//O ΣΥΝΟΛΙΚΟΣ ΑΡΙΘΜΟΣ ΤΩΝ POMODORO ΠΟΥ ΕΧΟΥΝ ΟΛΟΚΛΗΡΩΘΕΙ
		static int counter;
		//Ο ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΣ ΛΕΙΤΟΥΡΓΙΑΣ ΟΛΩΝ ΤΩΝ POMODORO
		static int totalTime;
		
		
	public:
		//ΜΕΤΑΒΛΗΤΗ ΤΗΣ ΚΛΑΣΗΣ ΠΟΥ ΚΡΑΤΑΕΙ
		//ΤΟΝ ΑΡΙΘΜΟ ΤΩΝ ΣΥΝΟΔΩΝ ΠΟΥ ΕΧΟΥΝ ΟΛΟΚΛΗΡΩΘΕΙ
		static int sessionsCompleted;
		//ΜΕΤΑΒΛΗΤΗ ΤΗΣ ΚΛΑΣΗΣ ΠΟΥ ΚΡΑΤΑΕΙ
		//ΤΟΝ ΣΥΝΟΛΙΚΟ ΧΡΟΝΟ ΕΡΓΑΣΙΑΣ ΟΛΩΝ ΤΩΝ ΑΝΤΙΚΕΙΜΕΝΩΝ
		static int totalWorkTime;
		
		//CONSTRUCTORS
		Pomodoro();
		Pomodoro(int wd, int bd);
		//DE-CONSTRUCTORS
		~Pomodoro();
		//ΑΡΧΙΖΕΙ ΕΝΑ ΝΕΟ SESSION
		void startSession();
		//ΟΛΟΚΛΗΡΩΝΕΙ ΕΝΑ SESSION
		void endSession();
		//ΔΙΑΚΟΠΤΕΙ ΤΗ ΛΕΙΤΟΥΡΓΙΑ ΤΟΥ SESSION ΠΡΟΣΩΡΙΝΑ
		void pause();
		//ΣΥΝΕΧΙΣΖΕΙ ΤΗ ΛΕΙΤΟΥΡΓΙΑ SESSION ΠΟΥ ΕΙΧΕ ΔΙΑΚΟΠΕΙ
		void resume();
		//ΕΠΙΣΤΕΦΕΙ ΣΕ ΣΥΜΒΟΛΟΣΕΙΡΑ ΤΟΝ ΧΡΟΝΟ ΛΕΙΤΟΥΡΓΙΑΣ ΚΑΙ ΔΙΑΛΕΙΜΜΑΤΟΣ ΠΟΥ ΕΧΕΙ ΠΕΡΑΣΕΙ
		char *showTime();
		//ΠΡΟΣΟΜΟΙΑΖΕΙ ΤΗ ΛΕΙΤΟΥΡΓΙΑ ΤΟΥ POMODORO
		int run();
		//SETTERS - GETTERS
		void setWorkDuration(int x);
		void setBreakDuration(int x);
		int getWorkDuration();
		int getBreakDuration();
		void setChronoStarted(int x);
		int getChronoStarted();
		void setChronoEnd(int x);
		int getChronoEnd();
		void setStatus(int x);
		int getStatus();
		int getWorkTimeElapsed();
		void setWorkTimeElapsed(int x);
		int getId();
		void setId(int x);
		int getBreakTimeElapsed();
		void setBreakTimeElapsed(int x);
		static int getCounter();
		static int getTotalTime();
		static void setCounter();
		static void setTotalTime();
	
};

#endif
