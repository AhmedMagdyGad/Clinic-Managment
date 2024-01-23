/************************************************************************************************************
 * @file   : admin_mode.h.
 * @author : Ahmed Magdy Gad.
 * @link   : https://www.linkedin.com/in/ahmed-gad-05b296272/ .
*************************************************************************************************************/

#ifndef ADMIN_MODE_H_INCLUDED
#define ADMIN_MODE_H_INCLUDED


/* --------------   Includes Section -------------- */

#include "Data_Types.h"

/* --------------   Macro Defenition Section -------------- */

#define MAX_TRAILS  3
#define ADMIN_MODE  1
#define USER_MODE   2
#define CLOSE       3
#define EDIT_ID     1
#define EDIT_SLOT   2

/* --------------   Macro Function Section -------------- */

/* --------------   Datatype Section -------------- */

typedef struct {
    uint8_t name[50];
    uint8_t date[11];
    uint8_t slot[5]; 
    uint32_t age;
    uint32_t ID;
    uint8_t Gender;
}Patient_Inf_t;

extern uint8_t pass[];

/* --------------   Function Decleration Section -------------- */

/************************************************************************************************************
 * @brief  : Function to get the password from the Admin and he has Only 3 trials.
 * @param  : It takes pointer to the pass[] to compare it with the Admin password.
 * @return : It returns state of the function if the user successfully enterd the password right or not.
 *           If the function returns E_NOT_OK -> then the Admin has exceded his trails. 
 *           If the function returns E_OK     -> then the Admin has entered the password right.
*************************************************************************************************************/
uint8_t Getting_the_password (uint8_t* pass);

/************************************************************************************************************
 * @brief  : Function to get the patient informations and stores it in a DataBase File.
 * @param  : it takes nothing as a parameter (but it takes alot from the Admin 😅).
 * @return : No returning.
*************************************************************************************************************/
void getting_patient_information(void);

/************************************************************************************************************
 * @brief  : Function to Edit the patient informations and restores it in a DataBase File.
 * @param  : it takes nothing as a parameter.
 * @return : No returning.
*************************************************************************************************************/
void Editting_patient_information(void);

/************************************************************************************************************
 * @brief  : Function to Cancel the patient informations and deletes it from the DataBase File.
 * @param  : it takes nothing as a parameter (but it takes the ID from the Admin).
 * @return : No returning.
*************************************************************************************************************/
void Cancel_patient_reservation(void);

#endif // ADMIN_MODE_H_INCLUDED
