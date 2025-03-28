#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "Pomodoro.hpp"


int defaultDuration;//ΠΡΟΚΑΘΟΡΙΣΜΕΝΗ ΔΙΑΡΚΕΙΑ ΕΡΓΑΣΙΑΣ
int defaultBreak;// ΠΡΟΚΑΘΟΡΙΣΜΕΝΗ ΔΙΑΡΚΕΙΑ ΔΙΑΛΕΙΜΜΑΤΟΣ
int pomodoroStatus; //Η ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO
char *buf; //BUFFER ΠΟΥ ΧΡΗΣΙΜΟΠΟΙΕΊΤΑΙ ΣΤΗΝ ΕΠΙΚΟΙΝΩΝΙΑ ΜΕΣΩ ΚΡΥΦΗΣ ΜΝΗΜΗΣ
key_t key, key1;//ΤΥΠΟΣ ΜΕΤΑΒΛΗΤΗΣ ΓΙΑ ΤΑ ΚΛΕΙΔΙΑ ΤΗς ΚΟΙΝΗΣ ΜΝΗΜΗΣ ΑΦΟΥ ΧΡΗΣΙΜΟΠΟΙΩ FTOK(), INT/LONG
int shmid; //ΑΝΑΓΝΩΡΙΣΤΙΚΟ ΚΡΥΦΗΣ ΜΝΗΜΗΣ
int shmid1; //ΑΝΑΓΝΩΡΙΣΤΙΚΟ ΚΡΥΦΗΣ ΜΝΗΜΗΣ
Pomodoro *P;// ΔΕΙΚΤΗΣ ΣΤΗΝ ΣΥΝΟΔΟ ΠΟΥ ΕΚΤΕΛΕΙΤΑΙ ΚΑΘΕ ΦΟΡΑ
int pidTime; //ΤΟ ΑΝΑΓΝΩΡΙΣΤΙΚΟ ΤΗΣ ΔΙΕΡΓΑΣΙΑΣ ΠΟΥ ΠΑΡΟΥΣΙΑΖΕΙ ΤΟ ΡΟΛΟΪ
WINDOW *tw; //ΤΟ ΠΑΡΑΘΥΡΟ ΤΟΥ ΧΡΟΜΕΤΡΟΥ
//ΔΙΑΙΡΕΙ ΤΗΝ ΓΡΑΜΜΑΤΟΣΕΙΡΑ ΣΕ 7 ΣΤΟΙΧΕΙΑ ΠΟΥ ΧΩΡΙΖΟΝΤΑΙ ΜΕ ΤΟΝ ΧΑΡΑΚΤΗΡΑ ":"
void splitStatsString(char *str, char x1[10], char x2[10],char x3[10], char x4[10],char x5[10], char x6[10], char x7[10]){
	//ΔΗΜΙΟΥΡΓΙΑ ΑΝΤΙΓΡΑΦΟΥ ΤΗΣ ΜΕΤΑΒΛΗΤΗΣ ΕΙΣΟΔΟΥ
	char *str1 = (char *)malloc(strlen(str)+1);
	strcpy(str1,str);
	//ΑΡΧΙΚΟΠΟΙΗΣΗ ΤΩΝ ΤΜΗΜΑΤΩΝ ΤΗΣ ΕΙΣΟΔΟΥ
	sprintf(x1,"%s","0");
	sprintf(x2,"%s","0");
	sprintf(x3,"%s","0");
	sprintf(x4,"%s","0");
	sprintf(x5,"%s","-");
	sprintf(x6,"%s","0");
	sprintf(x7,"%s","0");
	//ΔΙΑΣΠΑΣΗ ΤΗΣ ΕΙΣΟΔΟΥ
	char *p;
	p = strtok(str1,":");
	if (p!=NULL) {
		sprintf(x1,"%s",p);
		p = strtok(NULL,":");
		if (p!=NULL){
			sprintf(x2,"%s",p);
			p = strtok(NULL,":");
			if (p!=NULL){
				sprintf(x3,"%s",p);
				p = strtok(NULL,":");
				if (p!=NULL){
					sprintf(x4,"%s",p);
					p = strtok(NULL,":");
					if (p!=NULL){
						sprintf(x5,"%s",p);
						p = strtok(NULL,":");
						if (p!=NULL){
							sprintf(x6,"%s",p);
							p = strtok(NULL,":");
							if (p!=NULL){
								sprintf(x7,"%s",p);
							}
						}
					}
				}
			}
		}
	}
}

// ΔΗΜΙΟΥΡΓΙΑ ΜΕΝΟΥ ΜΕ ΕΠΙΛΟΓΕΣ ΣΤΟ ΤΕΡΜΑΤΙΚΟ 
WINDOW *menu(int h, int w, int y, int x){
	//ΕΠΙΚΟΙΝΩΝΙΑ ΜΕ 2Η ΔΙΕΡΓΑΣΙΑ
	char totalWtime[10]; //ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΝΟΣ ΛΕΙΤΟΥΡΓΙΑΣ ΤΩΝ 
	char totalSessions[10];//ΣΥΝΟΛΙΚΟΣ ΑΡΙΘΜΟΣ SESSIONS POY EXOYN ΟΛΟΚΛΗΩΘΕΙ
	char currentWtime[10]; //Ο ΧΡΟΝΟΣ ΠΟΥ ΛΕΙΤΟΥΡΓΕΙ ΤΟ ΤΡΕΧΟΝ SESSION
	char currentBtime[10]; //O ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΣ ΔΙΑΛΕΙΜΜΑΤΟΣ ΤΟΥ ΤΡΕΧΟΝΤΟΣ SESSION
	char dur[10]; //Η DEFAULT ΔΙΑΡΚΕΙΑ ΤΟΥ POMOFORO SESSION
	char breaks[10]; //H DEFAULT ΔΙΑΡΚΕΙΑ ΤΩΝ ΔΙΑΛΕΙΜΜΑΤΩΝ ΤΩΝ POMODORO SESSION
	char currentStatus[10]; //Η ΤΡΕΧΟΥΣΑ ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO
	//ΑΝΑΚΤΗΣΗ ΔΕΔΟΜΕΝΩΝ ΑΠΟ ΚΟΙΝΗ ΜΝΗΜΗ
	buf = (char *)shmat(shmid, (void*)0,0);
	splitStatsString(buf, currentWtime, currentBtime,  totalSessions, totalWtime, dur, breaks, currentStatus);
	shmdt(buf);
	//ΕΝΗΜΕΡΩΝΕΤΑΙ Η ΤΡΕΧΟΥΣΑ ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO
	pomodoroStatus = atoi(currentStatus);
	//ΔΗΛΩΣΗ ΠΑΡΑΘΥΡΟΥ
	WINDOW *menuWindow;
	//ΔΗΜΙΟΥΡΓΙΑ ΤΟΥ ΠΑΡΑΘΥΡΟΥ ΤΟΥ ΜΕΝΟΥ
	menuWindow = newwin(h,w,y,x);//ΔΗΜΙΟΥΡΙΑ ΠΑΡΑΘΥΡΟΥ
	box(menuWindow, 0, 0);	
	refresh();
	//ΠΑΡΟΥΣΙΑΣΗ ΕΠΙΛΟΓΩΝ
	mvwprintw(menuWindow,1,1,"%s","1. New Session");
	mvwprintw(menuWindow,2,1,"%s","2. Statistics");
	mvwprintw(menuWindow,3,1,"%s","3. Time Configuration");
	if ((pomodoroStatus == STATUS_STARTED)||(pomodoroStatus == STATUS_RESUMED)){
		mvwprintw(menuWindow,4,1,"%s","8. Pause");
	}
	if (pomodoroStatus == STATUS_PAUSED){
		mvwprintw(menuWindow,4,1,"%s","9. Resume");
	}
	mvwprintw(menuWindow,6,1,"%s","0. Exit");
	mvwprintw(menuWindow,12,1,"%s","->");
	wrefresh(menuWindow);//ΕΜΦΑΝΙΣΗ ΑΛΛΑΓΩΝ ΣΤΗΝ ΟΘΟΝΗ
	refresh();
	return menuWindow;
}

//ΣΥΝΑΡΤΗΣΗ ΣΤΗΝ ΟΠΟΙΑ ΟΡΙΖΕΙ Ο ΧΡΗΣΤΗΣ ΤΟΥΣ ΧΡΟΝΟΥΣ ΔΟΥΛΕΙΑΣ ΚΑΙ ΔΙΑΛΕΙΜΜΑΤΟΣ
void configuration(int h, int w, int y, int x, int *d, int *b){
	char par1[10];
	char par2[10];
	char *buf1;
	WINDOW *confWindow;
	confWindow = newwin(10,30,1,31);
	refresh();
	box(confWindow, 0, 0);
	keypad(confWindow, TRUE);
	echo();
	//ΜΗΝΥΜΑ ΠΡΟΣ ΧΡΗΣΤΗ ΓΙΑ ΤΗΝ ΕΙΣΑΓΩΓΗ ΝΕΟΥ ΧΡΟΝΟΥ ΔΟΥΛΕΙΑΣ
	mvwprintw(confWindow,1,1,"%s","New Working Time:");
	wrefresh(confWindow);
	mvwgetnstr(confWindow, 1, 20, par1, 9);
	mvwprintw(confWindow,3,1,"%s","New Break Time:");
	wrefresh(confWindow);
	mvwgetnstr(confWindow, 3, 20, par2, 9);
	//ΕΝΗΜΕΡΩΣΗ ΤΩΝ ΤΙΜΩΝ ΤΩΝ ΜΕΑΤΒΛΗΤΩΝ ΤΩΝ ΠΑΡΑΜΕΤΡΩΝ
	*d=atoi(par1);
	*b=atoi(par2);
	wrefresh(confWindow);
	refresh();
}

// ΣΥΝΑΡΤΗΣΗ ΠΟΥ ΕΜΦΑΝΙΖΕΙ ΠΑΡΑΘΥΡΟ ΤΟΝ ΧΡΟΝΟ ΠΟΥ ΤΡΕΧΕΙ ΜΙΑ ΣΥΝΟΔΟΣ
WINDOW *timeElapsed(int h, int w, int y, int x, Pomodoro *P){
	char totalWtime[10]; //ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΝΟΣ ΛΕΙΤΟΥΡΓΙΑΣ ΤΩΝ 
	char totalSessions[10];//ΣΥΝΟΛΙΚΟΣ ΑΡΙΘΜΟΣ SESSIONS POY EXOYN ΟΛΟΚΛΗΩΘΕΙ
	char currentWtime[10]; //Ο ΧΡΟΝΟΣ ΠΟΥ ΛΕΙΤΟΥΡΓΕΙ ΤΟ ΤΡΕΧΟΝ SESSION
	char currentBtime[10]; //O ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΣ ΔΙΑΛΕΙΜΜΑΤΟΣ ΤΟΥ ΤΡΕΧΟΝΤΟΣ SESSION
	char dur[10]; //
	char breaks[10]; //
	char currentStatus[10]; //Η ΤΡΕΧΟΥΣΑ ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO	
	//ΛΑΜΒΑΝΕΤΑΙ ΑΠΟ ΤΗΝ ΚΟΙΝΗ ΜΝΗΜΗ ΕΝΗΜΕΡΩΣΗ ΓΙΑ ΤΗΝ ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO
	char *buf_child = (char *)shmat(shmid1, (void*)0,0);
	splitStatsString(buf_child, currentWtime, currentBtime,  totalSessions, totalWtime, dur, breaks, currentStatus);
	shmdt(buf_child);
	//ΕΜΦΑΝΙΣΗ ΤΟΥ ΧΡΟΝΟΥ ΣΤΗΝ ΟΘΟΝΗ
	WINDOW *timeWindow;
	timeWindow = newwin(h,w,y,x);
	box(timeWindow, 0, 0);
	refresh();
	//ΕΜΦΑΝΙΣΗ ΧΡΟΝΟΥ ΓΙΑ ΤΗΝ ΣΥΝΟΔΟΔΟ Χ
	mvwprintw(timeWindow,1,1,"Working Time: %s",currentWtime);
	mvwprintw(timeWindow,2,1,"Break   Time: %s",currentBtime);
	wrefresh(timeWindow);
	refresh();
	return timeWindow;
}

//ΣΥΝΑΡΤΗΣΗ ΠΟΥ ΔΕΙΧΝΕΙ ΤΑ ΣΤΑΤΙΣΤΙΚΑ(ΠΛΗΘΟΣ ΣΥΝΕΔΡΙΩΝ/ΣΥΝΝΟΛΙΚΟΣ ΧΡΟΝΟΣ ΕΡΓΑΣΙΑΣ)
void stats(){
	char totalWtime[10]; //ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΝΟΣ ΛΕΙΤΟΥΡΓΙΑΣ ΤΩΝ 
	char totalSessions[10];//ΣΥΝΟΛΙΚΟΣ ΑΡΙΘΜΟΣ SESSIONS POY EXOYN ΟΛΟΚΛΗΩΘΕΙ
	char currentWtime[10]; //Ο ΧΡΟΝΟΣ ΠΟΥ ΛΕΙΤΟΥΡΓΕΙ ΤΟ ΤΡΕΧΟΝ SESSION
	char currentBtime[10]; //O ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΣ ΔΙΑΛΕΙΜΜΑΤΟΣ ΤΟΥ ΤΡΕΧΟΝΤΟΣ SESSION
	char dur[10]; //
	char breaks[10]; //
	char currentStatus[10]; //Η ΤΡΕΧΟΥΣΑ ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO
	//ΕΝΗΜΕΡΩΣΗ ΓΙΑ ΤΗΝ ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO ΑΠΟ ΤΗΝ ΚΟΙΝΗ ΜΝΗΜΗ
	buf = (char *)shmat(shmid, (void*)0,0);
	splitStatsString(buf, currentWtime, currentBtime,  totalSessions, totalWtime, dur, breaks, currentStatus);
	shmdt(buf);
	//ΕΜΦΑΝΙΣΗ ΤΟΥ ΠΑΡΑΘΥΡΟΥ ΤΩΝ ΣΤΑΤΙΣΤΙΚΩΝ
	WINDOW *statsWindow;
	statsWindow = newwin(10,30,1,31);
	refresh();
	box(statsWindow, 0, 0);
	//ΜΗΝΥΜΑ ΠΡΟΣ ΧΡΗΣΤΗ ΓΙΑ ΤΗΝ ΕΙΣΑΓΩΓΗ ΝΕΟΥ ΧΡΟΝΟΥ ΔΟΥΛΕΙΑΣ
	mvwprintw(statsWindow,1,1,"Sessions terminated: %s",totalSessions);
	mvwprintw(statsWindow,2,1,"Total Working Time: %s (%s)",totalWtime, dur);
	wrefresh(statsWindow);
}

//ΛΑΜΒΑΝΕΙ ΣΗΜΑ ΑΠΟ ΤΗΝ ΓΟΝΙΚΗ ΔΙΕΡΓΑΣΙΑ ΚΑΙ ΑΝΑΛΟΓΑ
//ΚΑΝΕΙ PAUSE TO SESSION
void signalHandlerPause(int x){
	P->pause();
	//ΕΝΗΜΕΡΩΝΕΙ ΜΕΣΩ ΚΟΙΝΗΣ ΜΝΗΜΗΣ ΓΙΑ ΤΗΝ ΑΛΛΑΓΗ ΚΑΤΑΣΤΑΣΗΣ
	buf = (char *)shmat(shmid, (void*)0,0);
	sprintf(buf,"%d:%d:%d:%d:%d:%d:%d",P->getWorkTimeElapsed(),P->getBreakTimeElapsed(),Pomodoro::getCounter(),Pomodoro::getTotalTime(),P->getWorkDuration(),P->getBreakDuration(),P->getStatus());
	shmdt(buf);
}
//ΣΤΑΜΑΤΑΕΙ Η ΛΕΙΤΟΥΡΓΙΑ ΤΟΥ ΧΡΟΝΟΜΕΤΡΟΥ
void stopTimer(int x){
	wclear(tw);
	wrefresh(tw);
	kill(getpid(),SIGINT);
}
//ΧΕΙΡΙΖΕΤΑΙ ΣΗΜΕ SIGUSR2 ΓΙΑ ΕΚΚΙΝΗΣΗ Η ΣΥΝΕΧΙΣΗ SESSION
void signalHandlerResume(int x){
	//AN TO SESSION ΗΤΑΝ ΣΕ ΚΑΤΑΣΤΑΣΗ ΑΝΑΜΟΝΗΣ ΕΚΚΙΝΕΙ
	if (P->getStatus() == STATUS_HOLD){
		P->startSession();
		//ΔΗΜΙΟΥΡΓΕΙΤΑΙ ΝΕΑ ΔΙΕΡΓΑΣΙΑ ΓΙΑ ΤΗΝ ΠΡΟΒΟΛΗ ΤΟΥ ΧΡΟΝΟΜΕΤΡΟΥ
		pidTime = fork();
		if (pidTime == 0){
			//ΑΝΑΜΕΝΕΙ ΣΗΜΑ ΓΙΑ ΤΕΡΜΑΤΙΣΜΟ
			signal(SIGUSR2, stopTimer);
			//ΑΝΑΝΕΩΝΕΙ ΣΥΝΕΧΩΣ ΤΟ ΧΡΟΝΟΜΕΤΡΟ
			while(1){
				tw = timeElapsed(10, 30, 1, 70,P);
				sleep(1);
			}
		}
	}
	//ΕΠΑΝΑΛΑΜΒΑΝΕΙ ΤΙΣ ΙΔΙΕΣ ΕΝΕΡΓΕΙΕΣ ΑΝ Η ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO
	//ΔΕΙΧΝΕΙ ΟΤΙ ΈΧΕΙ ΤΕΡΜΑΤΙΣΕΙ ΑΠΟ ΠΡΟΗΓΟΥΜΕΝΟ SESSION
	else if (P->getStatus() == STATUS_END){
		P->startSession();
		pidTime = fork();
		if (pidTime == 0){
			signal(SIGUSR2, stopTimer);
			while(1){
				tw = timeElapsed(10, 30, 1, 70,P);
				sleep(1);
			}
		}
	}
	//AN TO SESSION ΗΤΑΝ ΣΕ ΚΑΤΑΣΤΑΣΗ PAUSED ΠΕΡΝΑΕΙ ΣΕ ΚΑΤΑΣΤΑΣΗ RESUMED
	else if (P->getStatus() == STATUS_PAUSED){
		P->resume();
	}
	//ΕΝΗΜΕΡΩΣΗ ΜΕΣΩ ΤΗΣ ΚΡΥΦΗΣ ΜΝΗΜΗΣ ΓΙΑ ΤΗΝ ΑΛΛΑΓΗ ΚΑΤΑΣΤΑΣΗΣ
	buf = (char *)shmat(shmid, (void*)0,0);
	sprintf(buf,"%d:%d:%d:%d:%d:%d:%d",P->getWorkTimeElapsed(),P->getBreakTimeElapsed(),Pomodoro::getCounter(),Pomodoro::getTotalTime(),P->getWorkDuration(),P->getBreakDuration(),P->getStatus());
	shmdt(buf);
}
//ΧΕΙΡΙΖΕΤΑΙ ΣΗΜΑ ΓΙΑ ΤΟΝ ΤΕΡΜΑΤΙΣΜΟ ΤΗΣ ΕΦΑΡΜΟΓΗΣ
void signalForTerminating(int x){
	//ΤΕΡΜΑΤΙΖΕΙ ΤΟ ΧΡΟΝΟΜΕΤΡΟ
	kill(pidTime, SIGINT);
	//ΔΙΑΓΡΑΦΕΙ ΤΟ POMODORO SESSION
	delete P;
	//ΤΕΡΜΑΤΙΖΕΙ ΤΟΝ ΕΑΥΤΟ ΤΗΣ
	kill(getpid(),SIGINT);
}
//ΤΟ ΚΥΡΙΟ ΠΡΟΓΡΑΜΜΑ
int main(int argc, char** argv) {
	//ΟΡΙΖΟΝΤΑΙ Ο DEFAULT ΧΡΟΝΟΙ ΛΕΙΤΟΥΡΓΙΑΣ ΚΑΙ ΔΙΑΛΕΙΜΜΑΤΟΣ
	defaultDuration = 25;
	defaultBreak = 5;
	//Η ΑΡΧΙΚΗ ΚΑΤΑΣΤΑΣΗ ΕΙΝΑΙ ΑΝΑΜΟΝΗ
	pomodoroStatus = STATUS_HOLD;
	//ΔΗΜΙΟΥΡΓΙΑ 2 ΚΟΜΜΑΤΙΩΝ ΚΟΙΝΗΣ ΜΝΗΜΗΣ ΓΙΑ ΕΠΙΚΟΙΝΩΝΙΑ ΤΩΝ ΔΥΟ ΔΙΕΡΓΑΣΙΩΝ
	key = ftok("shmfile",65);
	key1 = ftok("shmfile1",65);
	shmid  = shmget(key, 1024, 0777 | IPC_CREAT);
	shmid1 = shmget(key1, 1024, 0777 | IPC_CREAT);
	//ΚΑΤΑΓΡΑΦΕΤΑΙ Η ΑΡΧΙΚΗ ΚΑΤΑΣΤΑΣΗ ΤΗΣ ΕΦΑΡΜΟΓΉΣ ΣΤΗΝ ΚΟΙΝΗ ΜΝΗΜΗ
	char *buf_parrent;
	buf_parrent = (char *)shmat(shmid1, (void*)0,0);
	sprintf(buf_parrent,"0:0:0:0:-:0:0");
	shmdt(buf_parrent);	
	//ΑΡΧΙΚΟΠΟΙΕΊΤΑΙ ΤΟ ΠΑΡΑΘΥΡΟ ΤΟΥ ΜΕΝΟΥ
	WINDOW *W;
	initscr();
	//ΔΕΝ ΘΕΛΟΥΜΕ ΝΑ ΦΑΙΝΕΤΑΙ ΤΟ ΤΙ ΓΡΑΦΕΙ Ο ΧΡΗΣΤΗΣ
	noecho();
	curs_set( 0 );
	int request;
	//ΕΜΦΑΝΙΖΕΤΑΙ ΤΟ ΜΕΝΟΥ
	W = menu(15,30,1,1);
	refresh();
	//ΤΟ ΑΝΑΓΝΩΡΙΣΤΙΚΟ ΤΗΣ ΘΥΓΑΤΡΙΚΗΣ ΔΙΕΡΓΑΣΙΑ
	pid_t pid;
	//ΔΗΜΙΟΥΡΓΟΥΜΕ ΜΙΑ ΝΕΑ ΔΙΕΡΓΑΣΙΑ ΣΤΗΝ ΟΠΟΙΑ ΘΑ ΔΗΜΙΟΥΡΓΟΥΝΤΑΙ ΤΑ ΑΝΤΙΚΕΙΜΕΝΑ POMODORO
	//ΔΙΕΡΓΑΣΙΑ ΑΥΤΗ ΘΑ ΕΠΙΚΟΙΝΩΝΕΙ ΜΕ ΤΗΝ ΚΕΝΤΡΙΚΗ ΔΙΕΡΓΑΣΙΑ ΜΕ SHARED MEMORY ΚΑΙ SIGNALS
	pid = fork();
	if (pid < 0){
		cout << "Error creating child process!" << endl;
		exit(0);
	}
	else if (pid > 0){//ΚΕΝΤΡΙΚΗ ΔΙΕΡΓΑΣΙΑ
		//ΑΝΑΜΕΝΕΤΑΙ Η ΕΠΙΛΟΓΗ ΤΟΥ ΧΡΗΣΤΗ
		//ΒΡΟΧΟΣ ΠΟΥ ΤΡΕΧΕΙ ΩΣΠΟΥ Ο ΧΡΗΣΤΗΣ ΝΑ ΕΠΙΛΕΞΕΙ ΤΗΝ ΠΕΡΙΠΤΩΣΗ 0 (ASCII ΚΩΔΙΚΟΣ:48)
		request = mvwgetch(W, 12, 5);
		while (request != 48){
			switch (request){
				case 49: { //ΕΠΙΛΟΓΗ 1
					//ΕΝΗΜΕΡΩΝΕΙ ΤΗ ΘΥΓΑΤΡΙΚΗ ΔΙΕΡΓΑΣΙΑ ΓΙΑ ΤΟΥΣ DEFAULT ΧΡΟΝΟΥΣ
					buf = (char *)shmat(shmid1, (void*)0,0);
					sprintf(buf,"0:0:0:0:%d:%d:0",defaultDuration,defaultBreak);
					shmdt(buf);
					//ΑΠΟΣΤΟΛΗ ΣΗΜΑΤΟΣ ΓΙΑ ΤΗΝ ΕΝΑΡΞΗ ΝΕΟΥ SESSION
					kill(pid, SIGUSR2);
					W = menu(15,30,1,1);
					refresh();
					break;
				}
				case 50: {
					//ΔΙΑΒΑΖΕΙ ΑΠΟ ΤΗΝ ΚΟΙΝΗ ΜΝΗΜΗ ΤΙΣ ΠΛΗΡΟΦΟΡΙΕΣ ΠΟΥ ΓΡΑΦΕΙ Η ΘΥΓΑΤΡΙΚΗ ΔΙΕΡΓΑΣΙΑ
					stats();
					refresh();
					break;
				}
				case 51: {
					//ΕΜΦΑΝΙΖΕΙ ΤΗΝ ΟΘΟΝΗ ΚΑΤΑΧΩΡΗΣΗΣ ΤΩΝ ΝΕΩΝ ΣΤΟΙΧΕΙΩΝ
					configuration(10,30,1,1, &defaultDuration, &defaultBreak);
					buf = (char *)shmat(shmid1, (void*)0,0);
					sprintf(buf,"0:0:0:0:%d:%d:0",defaultDuration,defaultBreak);
					shmdt(buf);
					sleep(2);
					break;
				}
				case 56: {
					//ΣΤΕΛΝΕΙ ΣΗΜΑ ΣΤΗ ΘΥΓΡΑΤΡΙΚΗ ΔΙΕΡΓΑΣΙΑ ΓΙΑ PAUSE
					kill(pid,SIGUSR1);
					break;
				}
				case 57: {
					//ΣΤΕΛΝΕΙ ΣΗΜΑ ΣΤΗ ΔΙΕΡΓΑΣΙΑ ΓΙΑ RESUME
					kill(pid,SIGUSR2);
					break;
				}
				default: {
					break;
				}
			}
			sleep(1);
			wclear(W);
			W = menu(15,30,1,1);
			wrefresh(W);//ΕΜΦΑΝΙΣΗ ΑΛΛΑΓΩΝ ΣΤΗΝ ΟΘΟΝΗ
			refresh();
			request = mvwgetch(W, 12, 5);
		}
		endwin(); 
		kill(pid, SIGINT);
	}
	else {
		P = new Pomodoro();
		//ΧΕΙΡΙΣΜΟΣ ΣΗΜΑΤΟΣ ΓΙΑ PAUSE
		signal(SIGUSR1, signalHandlerPause);
		//ΧΕΙΡΙΣΜΟΣ ΣΗΜΑΤΟΣ ΓΙΑ RESUME
		signal(SIGUSR2, signalHandlerResume);
		//ΧΕΙΡΙΣΜΟΣ ΣΗΜΑΤΟΣ ΓΙΑ ΤΕΡΜΑΤΙΣΜΟ
		signal(SIGINT, signalForTerminating);
		//BUFFER ΓΙΑ ΤΗΝ ΕΠΙΚΟΙΝΩΝΙΑ ΜΕΣΩ ΚΡΥΦΗΣ ΜΝΗΜΗΣ
		char *buf_child;
		int i;
		while(1){
			//ΑΝ Η ΚΑΤΑΣΤΑΣΗ ΤΟΥ SESSION ΕΙΝΑΙ ΑΝΑΜΟΝΗ Ή ΤΕΡΜΑΤΙΣΜΟΣ --> ΕΚΚΙΝΗΣΗ
			if (P->getStatus()!=STATUS_HOLD && P->getStatus()!=STATUS_END){
				//ΓΙΑ ΟΣΟ ΤΡΕΧΕΙ ΤΟ POMODORO ΣΤΕΛΝΕΙ ΕΝΗΜΕΡΩΣΗ ΤΗΣ ΚΑΤΑΣΤΑΣΗΣ ΤΟΥ ΣΤΗ ΜΗΤΡΙΚΗ ΔΙΕΡΓΑΣΙΑ
				while (P->run()!=0){
					buf_child = (char *)shmat(shmid, (void*)0,0);
					sprintf(buf_child,"%d:%d:%d:%d:%d:%d:%d",P->getWorkTimeElapsed(),P->getBreakTimeElapsed(),Pomodoro::getCounter(),Pomodoro::getTotalTime(),P->getWorkDuration(),P->getBreakDuration(),P->getStatus());
					shmdt(buf_child);
				}
				//ΣΤΕΛΝΕΙ ΤΗΝ ΤΕΛΕΥΤΑΙΑ ΕΝΗΜΕΡΩΣΗ ΠΡΙΝ ΟΛΟΚΛΗΡΩΘΕΙ
				buf_child = (char *)shmat(shmid, (void*)0,0);
				sprintf(buf_child,"%d:%d:%d:%d:%d:%d:%d",P->getWorkTimeElapsed(),P->getBreakTimeElapsed(),Pomodoro::getCounter(),Pomodoro::getTotalTime(),P->getWorkDuration(),P->getBreakDuration(),P->getStatus());
				shmdt(buf_child);
				//ΤΕΡΜΑΤΙΖΕΙ ΤΗ ΔΙΕΡΓΑΣΙΑ ΤΟΥ ΧΡΟΝΟΜΕΤΡΟΥ
				kill(pidTime,SIGUSR2);
			}
			else {
				char totalWtime[10]; //ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΝΟΣ ΛΕΙΤΟΥΡΓΙΑΣ ΤΩΝ 
				char totalSessions[10];//ΣΥΝΟΛΙΚΟΣ ΑΡΙΘΜΟΣ SESSIONS POY EXOYN ΟΛΟΚΛΗΩΘΕΙ
				char currentWtime[10]; //Ο ΧΡΟΝΟΣ ΠΟΥ ΛΕΙΤΟΥΡΓΕΙ ΤΟ ΤΡΕΧΟΝ SESSION
				char currentBtime[10]; //O ΣΥΝΟΛΙΚΟΣ ΧΡΟΝΟΣ ΔΙΑΛΕΙΜΜΑΤΟΣ ΤΟΥ ΤΡΕΧΟΝΤΟΣ SESSION
				char dur[10]; //
				char breaks[10]; //
				char currentStatus[10]; //Η ΤΡΕΧΟΥΣΑ ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO	
				//ΛΑΜΒΑΝΕΙ ΕΝΗΜΕΡΩΣΗ ΑΠΟ ΤΗ ΜΗΤΡΙΚΗ ΔΙΕΡΓΑΣΙΑ ΓΙΑ ΤΙΣ DEFAULT TIMEΣ ΧΡΟΝΟΥ ΛΕΙΤΟΥΡΓΙΑΣ ΚΑΙ ΔΙΑΛΕΙΜΜΑΤΟΣ
				buf_child = (char *)shmat(shmid1, (void*)0,0);
				splitStatsString(buf_child, currentWtime, currentBtime,  totalSessions, totalWtime, dur, breaks, currentStatus);
				shmdt(buf_child);
				//ΑΝ ΔΕΝ ΕΙΜΑΣΤΕ ΣΤΗΝ ΑΡΧΗ ΤΟΤΕ ΕΝΗΜΕΡΩΝΟΝΤΑΙ ΟΙ DEFAULT TIMEΣ 
				if (strcmp(dur,"-")!=0){
					P->setWorkDuration(atoi(dur));
					P->setBreakDuration(atoi(breaks));
				}
			}
		
		}
	}
	cout << "\n";
	return 0;
}
