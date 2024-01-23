/************************************************************************************************************
 * @file   : user_mode.h.
 * @author : Ahmed Magdy Gad.
 * @link   : https://www.linkedin.com/in/ahmed-gad-05b296272/ .
*************************************************************************************************************/

#ifndef _USER_MODE_H_
#define _USER_MODE_H_

/* --------------   Includes Section -------------- */

#include "Data_Types.h"

/* --------------   Macro Defenition Section -------------- */

#define PATIENT_RECORD  1
#define RESERVATIONS    2
/* --------------   Macro Function Section -------------- */

/* --------------   Datatype Section -------------- */

typedef struct {
    char Date[11];
    char Slot[5];
    unsigned int ID;
}User_mode_t;

/* --------------   Function Decleration Section -------------- */

/************************************************************************************************************
 * @brief  : Function to print the patient record.
 * @param  : it takes nothing but requires the ID from the User.
 * @return : No returning.
*************************************************************************************************************/
void Print_Patient_Record(void);

/************************************************************************************************************
 * @brief  : Function to print the patient records on a specific day.
 * @param  : it takes nothing but requires the Date from the User.
 * @return : No returning.
*************************************************************************************************************/
void View_All_Recervations_Of_Specific_Day(void);

#endif 