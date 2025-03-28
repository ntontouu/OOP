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
		//Î£Î¥ÎÎŸÎ›Î™ÎšÎŸÎ£ Î§Î¡ÎŸÎÎŸÎ£ Î•Î¡Î“Î‘Î£Î™Î‘Î£
		int workDuration ; //25
		//Î£Î¥ÎÎŸÎ›Î™ÎšÎŸÎ£ Î§Î¡ÎŸÎÎŸÎ£ Î”Î™Î‘Î›Î•Î™ÎœÎœÎ‘Î¤ÎŸÎ£
		int breakDuration;  //5
		//Î§Î¡ÎŸÎÎŸÎ£ Î•Î¡Î“Î‘Î£Î™Î‘Î£ Î ÎŸÎ¥ Î•Î§Î•Î™ ÎšÎ‘Î¤Î‘ÎÎ‘Î›Î©Î˜Î•Î™
		int workTimeElapsed;
		//Î§Î¡ÎŸÎÎŸÎ£ Î”Î™Î‘Î›Î•Î™ÎœÎœÎ‘Î¤ÎŸÎ£ Î ÎŸÎ¥ Î•Î§Î•Î™ ÎšÎ‘Î¤Î‘ÎÎ‘Î›Î©Î˜Î•Î™
		int breakTimeElapsed;
		//Î§Î¡ÎŸÎÎ™ÎšÎ— Î£Î¤Î™Î“ÎœÎ— Î•ÎÎ‘Î¡ÎžÎ—Î£ Î§Î¡ÎŸÎÎŸÎœÎ•Î¤Î¡Î—Î£Î—Î£
		int chronoStarted;
		//Î§Î¡ÎŸÎÎ™ÎšÎ— Î£Î¤Î™Î“ÎœÎ— Î›Î—ÎžÎ—Î£ Î§Î¡ÎŸÎÎŸÎœÎ•Î¤Î¡Î—Î£Î—Î£
		int chronoEnd;
		//Î— ÎšÎ‘Î¤Î‘Î£Î¤Î‘Î£Î— Î£Î¤Î—Î ÎŸÎ ÎŸÎ™Î‘ Î’Î¡Î™Î£ÎšÎ•Î¤Î‘Î™ Î¤ÎŸ POMODORO
		int status;
		//TO ÎœÎŸÎÎ‘Î”Î™ÎšÎŸ Î‘ÎÎ‘Î“Î©ÎÎ¡Î™Î£Î¤Î™ÎšÎŸ Î¤ÎŸÎ¥ POMODORO
		int id;
		//O Î£Î¥ÎÎŸÎ›Î™ÎšÎŸÎ£ Î‘Î¡Î™Î˜ÎœÎŸÎ£ Î¤Î©Î POMODORO Î ÎŸÎ¥ Î•Î§ÎŸÎ¥Î ÎŸÎ›ÎŸÎšÎ›Î—Î¡Î©Î˜Î•Î™
		static int counter;
		//ÎŸ Î£Î¥ÎÎŸÎ›Î™ÎšÎŸÎ£ Î§Î¡ÎŸÎÎŸÎ£ Î›Î•Î™Î¤ÎŸÎ¥Î¡Î“Î™Î‘Î£ ÎŸÎ›Î©Î Î¤Î©Î POMODORO
		static int totalTime;
		
		
	public:
		//ÎœÎ•Î¤Î‘Î’Î›Î—Î¤Î— Î¤Î—Î£ ÎšÎ›Î‘Î£Î—Î£ Î ÎŸÎ¥ ÎšÎ¡Î‘Î¤Î‘Î•Î™
		//Î¤ÎŸÎ Î‘Î¡Î™Î˜ÎœÎŸ Î¤Î©Î Î£Î¥ÎÎŸÎ”Î©Î Î ÎŸÎ¥ Î•Î§ÎŸÎ¥Î ÎŸÎ›ÎŸÎšÎ›Î—Î¡Î©Î˜Î•Î™
		static int sessionsCompleted;
		//ÎœÎ•Î¤Î‘Î’Î›Î—Î¤Î— Î¤Î—Î£ ÎšÎ›Î‘Î£Î—Î£ Î ÎŸÎ¥ ÎšÎ¡Î‘Î¤Î‘Î•Î™
		//Î¤ÎŸÎ Î£Î¥ÎÎŸÎ›Î™ÎšÎŸ Î§Î¡ÎŸÎÎŸ Î•Î¡Î“Î‘Î£Î™Î‘Î£ ÎŸÎ›Î©Î Î¤Î©Î Î‘ÎÎ¤Î™ÎšÎ•Î™ÎœÎ•ÎÎ©Î
		static int totalWorkTime;
		
		//CONSTRUCTORS
		Pomodoro();
		Pomodoro(int wd, int bd);
		//DE-CONSTRUCTORS
		~Pomodoro();
		//Î‘Î¡Î§Î™Î–Î•Î™ Î•ÎÎ‘ ÎÎ•ÎŸ SESSION
		void startSession();
		//ÎŸÎ›ÎŸÎšÎ›Î—Î¡Î©ÎÎ•Î™ Î•ÎÎ‘ SESSION
		void endSession();
		//Î”Î™Î‘ÎšÎŸÎ Î¤Î•Î™ Î¤Î— Î›Î•Î™Î¤ÎŸÎ¥Î¡Î“Î™Î‘ Î¤ÎŸÎ¥ SESSION Î Î¡ÎŸÎ£Î©Î¡Î™ÎÎ‘
		void pause();
		//Î£Î¥ÎÎ•Î§Î™Î£Î–Î•Î™ Î¤Î— Î›Î•Î™Î¤ÎŸÎ¥Î¡Î“Î™Î‘ SESSION Î ÎŸÎ¥ Î•Î™Î§Î• Î”Î™Î‘ÎšÎŸÎ Î•Î™
		void resume();
		//Î•Î Î™Î£Î¤Î•Î¦Î•Î™ Î£Î• Î£Î¥ÎœÎ’ÎŸÎ›ÎŸÎ£Î•Î™Î¡Î‘ Î¤ÎŸÎ Î§Î¡ÎŸÎÎŸ Î›Î•Î™Î¤ÎŸÎ¥Î¡Î“Î™Î‘Î£ ÎšÎ‘Î™ Î”Î™Î‘Î›Î•Î™ÎœÎœÎ‘Î¤ÎŸÎ£ Î ÎŸÎ¥ Î•Î§Î•Î™ Î Î•Î¡Î‘Î£Î•Î™
		char *showTime();
		//Î Î¡ÎŸÎ£ÎŸÎœÎŸÎ™Î‘Î–Î•Î™ Î¤Î— Î›Î•Î™Î¤ÎŸÎ¥Î¡Î“Î™Î‘ Î¤ÎŸÎ¥ POMODORO
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