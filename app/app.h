/************************************************************************************************************
 * @file   : App.h.
 * @author : Ahmed Magdy Gad.
 * @link   : https://www.linkedin.com/in/ahmed-gad-05b296272/ .
*************************************************************************************************************/

#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

/* --------------   Includes Section -------------- */

#include "admin_mode.h"
#include "user_mode.h"

/* --------------   Macro Defenition Section -------------- */

/* --------------   Macro Function Section -------------- */

/* --------------   Datatype Section -------------- */

uint32_t mode = ZERO, Operation = ZERO;
uint8_t Return_value = ZERO;

/* --------------   Function Decleration Section -------------- */
void adminMode(void);
void userMode(void);

#endif // APPLICATION_H_INCLUDED
