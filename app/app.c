/************************************************************************************************************
 * @file   : App.c.
 * @author : Ahmed Magdy Gad.
 * @link   : https://www.linkedin.com/in/ahmed-gad-05b296272/ .
*************************************************************************************************************/

#include "app.h"
static inline void clearInputBuffer(void);

int main(){
    printf ("\nHello Welcome to our Clinic .... ");
    while (1){
        printf ("\n===========================================\n");
        printf ("1) Admin mode. \n");
        printf ("2) User mode.  \n");
        printf ("3) Exit.       \n");
        printf ("===========================================\n");
        printf ("Please choose the MODE that you need : ");
        scanf  ("%i", &mode);
        clearInputBuffer();
        switch(mode){
            case ADMIN_MODE: adminMode()                  ;  break;
            case USER_MODE : userMode()                   ;  break;
            case CLOSE     : exit(1)                      ;  break;
            default        : printf("Wrong Choice !! \n") ;  break;
        }
    }
    return 0;
}


void adminMode(void){
    printf ("Please enter the password (It consist of 4 digits): ");
    Return_value = Getting_the_password(pass);
    if (Return_value == E_NOT_OK){
        exit(1);
    }
    else {
        printf ("===========================================\n");
        printf ("1) Add new patient record. \n");
        printf ("2) Edit patient record.    \n");
        printf ("3) Cancel reservation.     \n");
        printf ("===========================================\n");
        printf ("Please Select your operation : ");
        scanf  ("%i", &Operation);
        clearInputBuffer();
        switch(Operation){
            case ADD    : getting_patient_information() ;   break;
            case EDIT   : Editting_patient_information();   break;
            case CANCEL : Cancel_patient_reservation()  ;   break;
            default     : printf("Invalid input\n")     ;   break;
        }
    }
}

void userMode(void){
    printf ("===========================================\n");
    printf ("1) View patient record.                 \n");
    printf ("2) View reservations of a specific Day. \n");
    printf ("===========================================\n");
    printf ("Please Select your operation : ");
    scanf  ("%i", &Operation);
    clearInputBuffer();
    switch(Operation){
        case PATIENT_RECORD : Print_Patient_Record()                 ;   break;
        case RESERVATIONS   : View_All_Recervations_Of_Specific_Day();   break;
        default             : printf("Invalid input\n")              ;   break;
    }
}

static inline void clearInputBuffer(void) {
   int c;
   while ((c = getchar()) != '\n' && c != EOF);
}
