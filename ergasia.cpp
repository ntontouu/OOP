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


int defaultDuration;//пяойахояислемг диаяйеиа еяцасиас
int defaultBreak;// пяойахояислемг диаяйеиа диакеиллатос
int pomodoroStatus; //г йатастасг тоу POMODORO
char *buf; //BUFFER поу вягсилопоие╨таи стгм епийоимымиа лесы йяужгс лмглгс
key_t key, key1;//тупос летабкгтгс циа та йкеидиа тгР йоимгс лмглгс ажоу вягсилопоиы FTOK(), INT/LONG
int shmid; //амацмыяистийо йяужгс лмглгс
int shmid1; //амацмыяистийо йяужгс лмглгс
Pomodoro *P;// деийтгс стгм сумодо поу ейтекеитаи йахе жояа
int pidTime; //то амацмыяистийо тгс диеяцасиас поу паяоусиафеи то яокоз
WINDOW *tw; //то паяахуяо тоу вяолетяоу
//диаияеи тгм цяаллатосеияа се 7 стоивеиа поу выяифомтаи ле том ваяайтгяа ":"
void splitStatsString(char *str, char x1[10], char x2[10],char x3[10], char x4[10],char x5[10], char x6[10], char x7[10]){
	//дглиоуяциа амтицяажоу тгс летабкгтгс еисодоу
	char *str1 = (char *)malloc(strlen(str)+1);
	strcpy(str1,str);
	//аявийопоигсг тым тлглатым тгс еисодоу
	sprintf(x1,"%s","0");
	sprintf(x2,"%s","0");
	sprintf(x3,"%s","0");
	sprintf(x4,"%s","0");
	sprintf(x5,"%s","-");
	sprintf(x6,"%s","0");
	sprintf(x7,"%s","0");
	//диаспасг тгс еисодоу
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

// дглиоуяциа лемоу ле епикоцес сто теялатийо 
WINDOW *menu(int h, int w, int y, int x){
	//епийоимымиа ле 2г диеяцасиа
	char totalWtime[10]; //сумокийос вяомомос кеитоуяциас тым 
	char totalSessions[10];//сумокийос аяихлос SESSIONS POY EXOYN окойкгыхеи
	char currentWtime[10]; //о вяомос поу кеитоуяцеи то тяевом SESSION
	char currentBtime[10]; //O сумокийос вяомос диакеиллатос тоу тяевомтос SESSION
	char dur[10]; //г DEFAULT диаяйеиа тоу POMOFORO SESSION
	char breaks[10]; //H DEFAULT диаяйеиа тым диакеиллатым тым POMODORO SESSION
	char currentStatus[10]; //г тяевоуса йатастасг тоу POMODORO
	//амайтгсг дедолемым апо йоимг лмглг
	buf = (char *)shmat(shmid, (void*)0,0);
	splitStatsString(buf, currentWtime, currentBtime,  totalSessions, totalWtime, dur, breaks, currentStatus);
	shmdt(buf);
	//емглеяыметаи г тяевоуса йатастасг тоу POMODORO
	pomodoroStatus = atoi(currentStatus);
	//дгкысг паяахуяоу
	WINDOW *menuWindow;
	//дглиоуяциа тоу паяахуяоу тоу лемоу
	menuWindow = newwin(h,w,y,x);//дглиоуяиа паяахуяоу
	box(menuWindow, 0, 0);	
	refresh();
	//паяоусиасг епикоцым
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
	wrefresh(menuWindow);//елжамисг аккацым стгм охомг
	refresh();
	return menuWindow;
}

//сумаятгсг стгм опоиа ояифеи о вягстгс тоус вяомоус доукеиас йаи диакеиллатос
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
	//лгмула пяос вягстг циа тгм еисацыцг меоу вяомоу доукеиас
	mvwprintw(confWindow,1,1,"%s","New Working Time:");
	wrefresh(confWindow);
	mvwgetnstr(confWindow, 1, 20, par1, 9);
	mvwprintw(confWindow,3,1,"%s","New Break Time:");
	wrefresh(confWindow);
	mvwgetnstr(confWindow, 3, 20, par2, 9);
	//емглеяысг тым тилым тым леатбкгтым тым паяалетяым
	*d=atoi(par1);
	*b=atoi(par2);
	wrefresh(confWindow);
	refresh();
}

// сумаятгсг поу елжамифеи паяахуяо том вяомо поу тяевеи лиа сумодос
WINDOW *timeElapsed(int h, int w, int y, int x, Pomodoro *P){
	char totalWtime[10]; //сумокийос вяомомос кеитоуяциас тым 
	char totalSessions[10];//сумокийос аяихлос SESSIONS POY EXOYN окойкгыхеи
	char currentWtime[10]; //о вяомос поу кеитоуяцеи то тяевом SESSION
	char currentBtime[10]; //O сумокийос вяомос диакеиллатос тоу тяевомтос SESSION
	char dur[10]; //
	char breaks[10]; //
	char currentStatus[10]; //г тяевоуса йатастасг тоу POMODORO	
	//калбаметаи апо тгм йоимг лмглг емглеяысг циа тгм йатастасг тоу POMODORO
	char *buf_child = (char *)shmat(shmid1, (void*)0,0);
	splitStatsString(buf_child, currentWtime, currentBtime,  totalSessions, totalWtime, dur, breaks, currentStatus);
	shmdt(buf_child);
	//елжамисг тоу вяомоу стгм охомг
	WINDOW *timeWindow;
	timeWindow = newwin(h,w,y,x);
	box(timeWindow, 0, 0);
	refresh();
	//елжамисг вяомоу циа тгм сумододо в
	mvwprintw(timeWindow,1,1,"Working Time: %s",currentWtime);
	mvwprintw(timeWindow,2,1,"Break   Time: %s",currentBtime);
	wrefresh(timeWindow);
	refresh();
	return timeWindow;
}

//сумаятгсг поу деивмеи та статистийа(пкгхос сумедяиым/суммокийос вяомос еяцасиас)
void stats(){
	char totalWtime[10]; //сумокийос вяомомос кеитоуяциас тым 
	char totalSessions[10];//сумокийос аяихлос SESSIONS POY EXOYN окойкгыхеи
	char currentWtime[10]; //о вяомос поу кеитоуяцеи то тяевом SESSION
	char currentBtime[10]; //O сумокийос вяомос диакеиллатос тоу тяевомтос SESSION
	char dur[10]; //
	char breaks[10]; //
	char currentStatus[10]; //г тяевоуса йатастасг тоу POMODORO
	//емглеяысг циа тгм йатастасг тоу POMODORO апо тгм йоимг лмглг
	buf = (char *)shmat(shmid, (void*)0,0);
	splitStatsString(buf, currentWtime, currentBtime,  totalSessions, totalWtime, dur, breaks, currentStatus);
	shmdt(buf);
	//елжамисг тоу паяахуяоу тым статистийым
	WINDOW *statsWindow;
	statsWindow = newwin(10,30,1,31);
	refresh();
	box(statsWindow, 0, 0);
	//лгмула пяос вягстг циа тгм еисацыцг меоу вяомоу доукеиас
	mvwprintw(statsWindow,1,1,"Sessions terminated: %s",totalSessions);
	mvwprintw(statsWindow,2,1,"Total Working Time: %s (%s)",totalWtime, dur);
	wrefresh(statsWindow);
}

//калбамеи сгла апо тгм цомийг диеяцасиа йаи амакоца
//йамеи PAUSE TO SESSION
void signalHandlerPause(int x){
	P->pause();
	//емглеяымеи лесы йоимгс лмглгс циа тгм аккацг йатастасгс
	buf = (char *)shmat(shmid, (void*)0,0);
	sprintf(buf,"%d:%d:%d:%d:%d:%d:%d",P->getWorkTimeElapsed(),P->getBreakTimeElapsed(),Pomodoro::getCounter(),Pomodoro::getTotalTime(),P->getWorkDuration(),P->getBreakDuration(),P->getStatus());
	shmdt(buf);
}
//сталатаеи г кеитоуяциа тоу вяомолетяоу
void stopTimer(int x){
	wclear(tw);
	wrefresh(tw);
	kill(getpid(),SIGINT);
}
//веияифетаи сгле SIGUSR2 циа еййимгсг г сумевисг SESSION
void signalHandlerResume(int x){
	//AN TO SESSION гтам се йатастасг амаломгс еййимеи
	if (P->getStatus() == STATUS_HOLD){
		P->startSession();
		//дглиоуяцеитаи меа диеяцасиа циа тгм пяобокг тоу вяомолетяоу
		pidTime = fork();
		if (pidTime == 0){
			//амалемеи сгла циа теялатисло
			signal(SIGUSR2, stopTimer);
			//амамеымеи сумевыс то вяомолетяо
			while(1){
				tw = timeElapsed(10, 30, 1, 70,P);
				sleep(1);
			}
		}
	}
	//епамакалбамеи тис идиес емеяцеиес ам г йатастасг тоу POMODORO
	//деивмеи оти ╦веи теялатисеи апо пяогцоулемо SESSION
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
	//AN TO SESSION гтам се йатастасг PAUSED пеямаеи се йатастасг RESUMED
	else if (P->getStatus() == STATUS_PAUSED){
		P->resume();
	}
	//емглеяысг лесы тгс йяужгс лмглгс циа тгм аккацг йатастасгс
	buf = (char *)shmat(shmid, (void*)0,0);
	sprintf(buf,"%d:%d:%d:%d:%d:%d:%d",P->getWorkTimeElapsed(),P->getBreakTimeElapsed(),Pomodoro::getCounter(),Pomodoro::getTotalTime(),P->getWorkDuration(),P->getBreakDuration(),P->getStatus());
	shmdt(buf);
}
//веияифетаи сгла циа том теялатисло тгс ежаялоцгс
void signalForTerminating(int x){
	//теялатифеи то вяомолетяо
	kill(pidTime, SIGINT);
	//диацяажеи то POMODORO SESSION
	delete P;
	//теялатифеи том еауто тгс
	kill(getpid(),SIGINT);
}
//то йуяио пяоцяалла
int main(int argc, char** argv) {
	//ояифомтаи о DEFAULT вяомои кеитоуяциас йаи диакеиллатос
	defaultDuration = 25;
	defaultBreak = 5;
	//г аявийг йатастасг еимаи амаломг
	pomodoroStatus = STATUS_HOLD;
	//дглиоуяциа 2 йоллатиым йоимгс лмглгс циа епийоимымиа тым дуо диеяцасиым
	key = ftok("shmfile",65);
	key1 = ftok("shmfile1",65);
	shmid  = shmget(key, 1024, 0777 | IPC_CREAT);
	shmid1 = shmget(key1, 1024, 0777 | IPC_CREAT);
	//йатацяажетаи г аявийг йатастасг тгс ежаялоц╧с стгм йоимг лмглг
	char *buf_parrent;
	buf_parrent = (char *)shmat(shmid1, (void*)0,0);
	sprintf(buf_parrent,"0:0:0:0:-:0:0");
	shmdt(buf_parrent);	
	//аявийопоие╨таи то паяахуяо тоу лемоу
	WINDOW *W;
	initscr();
	//дем хекоуле ма жаиметаи то ти цяажеи о вягстгс
	noecho();
	curs_set( 0 );
	int request;
	//елжамифетаи то лемоу
	W = menu(15,30,1,1);
	refresh();
	//то амацмыяистийо тгс хуцатяийгс диеяцасиа
	pid_t pid;
	//дглиоуяцоуле лиа меа диеяцасиа стгм опоиа ха дглиоуяцоумтаи та амтийеилема POMODORO
	//диеяцасиа аутг ха епийоимымеи ле тгм йемтяийг диеяцасиа ле SHARED MEMORY йаи SIGNALS
	pid = fork();
	if (pid < 0){
		cout << "Error creating child process!" << endl;
		exit(0);
	}
	else if (pid > 0){//йемтяийг диеяцасиа
		//амалеметаи г епикоцг тоу вягстг
		//бяовос поу тяевеи ыспоу о вягстгс ма епикенеи тгм пеяиптысг 0 (ASCII йыдийос:48)
		request = mvwgetch(W, 12, 5);
		while (request != 48){
			switch (request){
				case 49: { //епикоцг 1
					//емглеяымеи тг хуцатяийг диеяцасиа циа тоус DEFAULT вяомоус
					buf = (char *)shmat(shmid1, (void*)0,0);
					sprintf(buf,"0:0:0:0:%d:%d:0",defaultDuration,defaultBreak);
					shmdt(buf);
					//апостокг сглатос циа тгм емаянг меоу SESSION
					kill(pid, SIGUSR2);
					W = menu(15,30,1,1);
					refresh();
					break;
				}
				case 50: {
					//диабафеи апо тгм йоимг лмглг тис пкгяожояиес поу цяажеи г хуцатяийг диеяцасиа
					stats();
					refresh();
					break;
				}
				case 51: {
					//елжамифеи тгм охомг йатавыягсгс тым меым стоивеиым
					configuration(10,30,1,1, &defaultDuration, &defaultBreak);
					buf = (char *)shmat(shmid1, (void*)0,0);
					sprintf(buf,"0:0:0:0:%d:%d:0",defaultDuration,defaultBreak);
					shmdt(buf);
					sleep(2);
					break;
				}
				case 56: {
					//стекмеи сгла стг хуцяатяийг диеяцасиа циа PAUSE
					kill(pid,SIGUSR1);
					break;
				}
				case 57: {
					//стекмеи сгла стг диеяцасиа циа RESUME
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
			wrefresh(W);//елжамисг аккацым стгм охомг
			refresh();
			request = mvwgetch(W, 12, 5);
		}
		endwin(); 
		kill(pid, SIGINT);
	}
	else {
		P = new Pomodoro();
		//веияислос сглатос циа PAUSE
		signal(SIGUSR1, signalHandlerPause);
		//веияислос сглатос циа RESUME
		signal(SIGUSR2, signalHandlerResume);
		//веияислос сглатос циа теялатисло
		signal(SIGINT, signalForTerminating);
		//BUFFER циа тгм епийоимымиа лесы йяужгс лмглгс
		char *buf_child;
		int i;
		while(1){
			//ам г йатастасг тоу SESSION еимаи амаломг ╧ теялатислос --> еййимгсг
			if (P->getStatus()!=STATUS_HOLD && P->getStatus()!=STATUS_END){
				//циа осо тяевеи то POMODORO стекмеи емглеяысг тгс йатастасгс тоу стг лгтяийг диеяцасиа
				while (P->run()!=0){
					buf_child = (char *)shmat(shmid, (void*)0,0);
					sprintf(buf_child,"%d:%d:%d:%d:%d:%d:%d",P->getWorkTimeElapsed(),P->getBreakTimeElapsed(),Pomodoro::getCounter(),Pomodoro::getTotalTime(),P->getWorkDuration(),P->getBreakDuration(),P->getStatus());
					shmdt(buf_child);
				}
				//стекмеи тгм текеутаиа емглеяысг пяим окойкгяыхеи
				buf_child = (char *)shmat(shmid, (void*)0,0);
				sprintf(buf_child,"%d:%d:%d:%d:%d:%d:%d",P->getWorkTimeElapsed(),P->getBreakTimeElapsed(),Pomodoro::getCounter(),Pomodoro::getTotalTime(),P->getWorkDuration(),P->getBreakDuration(),P->getStatus());
				shmdt(buf_child);
				//теялатифеи тг диеяцасиа тоу вяомолетяоу
				kill(pidTime,SIGUSR2);
			}
			else {
				char totalWtime[10]; //сумокийос вяомомос кеитоуяциас тым 
				char totalSessions[10];//сумокийос аяихлос SESSIONS POY EXOYN окойкгыхеи
				char currentWtime[10]; //о вяомос поу кеитоуяцеи то тяевом SESSION
				char currentBtime[10]; //O сумокийос вяомос диакеиллатос тоу тяевомтос SESSION
				char dur[10]; //
				char breaks[10]; //
				char currentStatus[10]; //г тяевоуса йатастасг тоу POMODORO	
				//калбамеи емглеяысг апо тг лгтяийг диеяцасиа циа тис DEFAULT TIMEс вяомоу кеитоуяциас йаи диакеиллатос
				buf_child = (char *)shmat(shmid1, (void*)0,0);
				splitStatsString(buf_child, currentWtime, currentBtime,  totalSessions, totalWtime, dur, breaks, currentStatus);
				shmdt(buf_child);
				//ам дем еиласте стгм аявг тоте емглеяымомтаи ои DEFAULT TIMEс 
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