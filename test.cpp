#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "Pomodoro.hpp"
//ΕΛΕΓΧΟΣ ΑΡΧΙΚΟΠΟΙΗΣΗΣ ΧΡΟΝΟΥ ΔΙΑΛΕΙΜΜΑΤΟΣ
TEST_CASE( "Test Start Session: CHECK BREAK DURATION" ){    
        Pomodoro *P = new Pomodoro();
        REQUIRE(P->getBreakDuration()==5*60) ;
}
//ΕΛΕΓΧΟΣ ΑΡΧΙΚΟΠΟΙΗΣΗΣ ΧΡΟΝΟΥ ΛΕΙΤΟΥΡΓΙΑΣ
TEST_CASE( "Test Start Session: CHECK WORK DURATION" ){    
        Pomodoro *P = new Pomodoro();
        REQUIRE(P->getWorkDuration()==25*60) ;
    
}
//ΕΛΕΓΧΟΣ ΑΛΛΑΓΗΣ ΧΡΟΝΟΥ ΛΕΙΤΟΥΡΓΙΑΣ
TEST_CASE( "Test Change Work Duration" ){    
        Pomodoro *P = new Pomodoro();
        P->setWorkDuration(10);
        REQUIRE(P->getWorkDuration()==10*60) ;
}
//ΕΛΕΓΧΟΣ ΑΛΛΑΓΗΣ ΧΡΟΝΟΥ ΔΙΑΛΕΙΜΜΑΤΟΣ
TEST_CASE( "Test Change Break Duration" ){    
        Pomodoro *P = new Pomodoro();
        P->setBreakDuration(5);
        REQUIRE(P->getBreakDuration()==5*60) ;
}
//ΕΛΕΓΧΟΣ Ο ΟΛΙΚΟΣ ΧΡΟΝΟΣ ΛΕΙΤΟΥΡΓΙΑΣ ΝΑ ΜΗΝ ΕΙΝΑΙ ΑΡΝΗΤΙΚΟΣ ΑΡΙΘΜΟΣ
TEST_CASE("Total Time Correct"){
        Pomodoro *P = new Pomodoro();
        REQUIRE(P->getTotalTime()>=0);
}
//ΕΛΕΓΧΟΣ ΑΝ Η ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO EINAI STATUS_STARTED 
//ΚΑΤΑ ΤΗΝ ΕΝΑΡΞΗ ΤΟΥ SESSION
TEST_CASE("Correct Status Start"){
        Pomodoro *P = new Pomodoro();
        P->startSession();
        REQUIRE(P->getStatus()==STATUS_STARTED);
}
//ΕΛΕΓΧΟΣ ΑΝ Η ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO EINAI STATUS_END
//ΚΑΤΑ ΤΗΝ ΛΗΞΗ ΤΟΥ SESSION
TEST_CASE("Correct Status End"){
        Pomodoro *P = new Pomodoro();
        P->endSession();
        REQUIRE(P->getStatus()==STATUS_END);
}
//ΕΛΕΓΧΟΣ ΑΝ Η ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO EINAI STATUS_RESUMED 
//ΚΑΤΑ ΤΗΝ ΕΠΑΝΕΚΚΙΝΗΣΗ ΤΟΥ SESSION
TEST_CASE("Correct Status Resumed"){
        Pomodoro *P = new Pomodoro();
        P->resume();
        REQUIRE(P->getStatus()==STATUS_RESUMED);
}
//ΕΛΕΓΧΟΣ ΑΝ Η ΚΑΤΑΣΤΑΣΗ ΤΟΥ POMODORO EINAI STATUS_PAUSED 
//ΚΑΤΑ ΤΗ ΔΙΑΚΟΠΗ ΤΟΥ SESSION
TEST_CASE("Correct Status Paused"){
        Pomodoro *P = new Pomodoro();
        P->pause();
        REQUIRE(P->getStatus()==STATUS_PAUSED);
}
