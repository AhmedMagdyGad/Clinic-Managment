/************************************************************************************************************
 * @file   : admin_mode.c.
 * @author : Ahmed Magdy Gad.
 * @link   : https://www.linkedin.com/in/ahmed-gad-05b296272/ .
*************************************************************************************************************/

#include "admin_mode.h"

uint8_t pass[] = {"1234"}; /* Default Password */

/* The static Helper Functions prototypes used by this file. */
/************************************************************************************************************/
static inline void clearInputBuffer(void);
static uint8_t Compare_IDs(uint32_t  ID);
static uint8_t Compare_Date_and_Slot(const uint8_t Date[], const uint8_t Slot[]);
static void Editing_ID(void);
static void Editing_Date_and_Time(void);
static void Deleting_Specific_Line(uint32_t line);
static void Adding_A_new_Line_Changing_ID(uint8_t ID_Line_Data[], uint32_t New_ID);
static void Adding_New_Line_Changing_Date_Time(uint8_t ID_Line_Data[], uint8_t Date[], uint8_t Slot[]);
static void Print_Patient_Record(uint32_t ID);
/************************************************************************************************************/


/************************************************************************************************************
 * @brief  : Function to get the password from the Admin and he has Only 3 trials.
 * @param  : It takes pointer to the pass[] to compare it with the Admin password.
 * @return : It returns state of the function if the user successfully enterd the password right or not.
 *           If the function returns E_NOT_OK -> then the Admin has exceded his trails. 
 *           If the function returns E_OK     -> then the Admin has entered the password right.
*************************************************************************************************************/
uint8_t Getting_the_password (uint8_t *pass){

    uint8_t ret_val = E_NOT_OK;   /* variable to return the status of the function through it.  */
    uint8_t l_counter = ZERO;     /* local Counter for any loops will be used in this function. */
    uint8_t trials = MAX_TRAILS;  /* the 3 trails avilable for the Admin. */
    uint8_t flag = ZERO;          /* Flag to indicates if the two passwords are matched or not. */
    uint8_t Admin_Pass[5];        /* array of 5 elements to store the 4 digits of the password and the null termination '\0'. */
  
    if (NULL == pass){            /* Validation of the NULL pointer. */
        ret_val = E_NOT_OK;
    }
    else {
        while (trials){ 
            fgets (Admin_Pass, sizeof(Admin_Pass), stdin);  /* Getting the password. */
            clearInputBuffer();                             /* Clearing the buffer to avoid '\n'. */
            uint8_t len = strlen(Admin_Pass);               
            if (len > ZERO && Admin_Pass[len - 1] == '\n') {   /* adding null termination in the end of the array in case if there is a '\n'. */
                Admin_Pass[len - 1] = '\0';
            }
            /* Some animated Please wait .......... 😃😅 */
            printf("Please wait ");
            for(l_counter = ZERO; l_counter < 15; l_counter++){
                printf(".");
                Sleep(50);
            }
            printf("\n");
            /* Looping to compare if the two passwords are matching or NOT. */
            for (l_counter = ZERO; l_counter < 5; l_counter++){
                if (Admin_Pass[l_counter] != pass[l_counter]){
                    flag = 1; /* If any character dosn't match his opponent then the flag will be set and the loop will be broken. */
                    break;
                }
                else { /* Nothing */ }
            }
            /* If the flag = 1 then the trails will be decremented and the flag will be cleared and Admin will try again. */
            if (1 == flag){
                trials--; 
                printf ("Invalid Password !! \n");
                if (trials != 0){
                    printf ("Please Try again .. : ");    
                }
                else { printf ("This was your last chance\n");}
                flag = 0;
            }
            /* If the flag != 1 then the two passwords are matching and then the system will start. */
            else {
                printf ("Welcome :D \n");
                Sleep(150);
                ret_val = E_OK;
                break;
            }
        }
    }
    return (ret_val);
}

/************************************************************************************************************
 * @brief  : Function to get the patient informations and stores it in a DataBase File.
 * @param  : it takes nothing as a parameter (but it takes alot from the Admin 😅).
 * @return : No returning.
*************************************************************************************************************/
void getting_patient_information(void){
    uint8_t flag = 0; /* Flag for how many invalid ID has been entered the limit is 3.*/
    Patient_Inf_t Patient_Info; /* object of struct to store the information in it @ref Patient_Inf_t . */
    FILE *my_file = NULL;       /* Creating Pointer to FILE to creat the DataBase file to store the Data. */
    uint8_t Ret_Val = E_NOT_OK; /* variable to return the status of the function through it.  */
    my_file = fopen ("Data.txt", "a"); /* openning a file to modifiy on it and adding a new patient to our DateBase. */
ID_again:
    printf  ("\nPlease enter the patient ID : ");
    scanf_s ("%i", &Patient_Info.ID);  /* Getting the ID of the patient and must be unique. */
    clearInputBuffer();                /* Clearing the buffer to avoid '\n'. */
    Ret_Val = Compare_IDs(Patient_Info.ID); /* Calling a helper function to check if the ID is already exist or not. */
    if (Ret_Val == E_OK){ /* The ID is already taken. */
        printf ("Sorry this ID [%i] already exist. \n", Patient_Info.ID);
        flag++;
        if (flag != 3){
            goto ID_again;
        }
        else {
            printf("You have reached the Limit.\n");
        }
    }
    else { /* If the ID is available. */
    Date_again:
       printf  ("Please enter the date (in this Format \"00/00/0000\") : ");
       fgets   (Patient_Info.date, sizeof(Patient_Info.date), stdin); /* Getting the Date of the reservation. */
       clearInputBuffer();                                            /* Clearing the buffer to avoid '\n'. */
       printf  ("Please enter the slot (Choose from these [02:00 pm - 02:30 pm - 03:30 pm - 04:00 pm - 04:30 pm] ) : ");
       fgets   (Patient_Info.slot, sizeof(Patient_Info.slot), stdin); /* Getting the Time of the reservation. */
       clearInputBuffer();                                            /* Clearing the buffer to avoid '\n'. */
       /* Calling a helper function to check if this time is available or not*/
       Ret_Val = Compare_Date_and_Slot(Patient_Info.date, Patient_Info.slot); 
       if (Ret_Val == E_OK){ /*The Slot is already taken. */
           printf("This Slot [Data : %s & Slot = %s0] already taken.\n", Patient_Info.date, Patient_Info.slot);
           flag++;
            if (flag != 3){
                goto Date_again;
            }
            else {
                printf("You have reached the Limit.\n");
            }
       }
       else { /* The Slot is Free. */
           printf  ("Please enter the patient Age : ");
           scanf_s ("%i", &Patient_Info.age); /* Getting the age of the patient. */
           clearInputBuffer();                /* Clearing the buffer to avoid '\n'. */
           printf  ("Please enter the patient Gender [M/F]: ");
           scanf_s ("%c", &Patient_Info.Gender);    /* Getting the gender of the patient. */
           clearInputBuffer();                      /* Clearing the buffer to avoid '\n'. */
           printf  ("Please enter the patient Name : ");
           fgets   (Patient_Info.name, sizeof(Patient_Info.name), stdin); /* Getting the patient's name. */
           /* Storing the information in This Format. */
           fprintf (my_file, "ID: %i   - Date: %s - Slot: %s0 - Age: %i - Gender: %c - Name: %s\n", 
                       Patient_Info.ID,  Patient_Info.date, Patient_Info.slot, Patient_Info.age, Patient_Info.Gender, Patient_Info.name);
           printf  ("\nDone, the patient has been successfully added to the DataBase. \n");
           fclose  (my_file); /* Closing the file after finishing. */
       }
    }
}

/************************************************************************************************************
 * @brief  : Function to Edit the patient informations and restores it in a DataBase File.
 * @param  : it takes nothing as a parameter.
 * @return : No returning.
*************************************************************************************************************/
void Editting_patient_information(void){
    printf("\n=========================================== \n");
    printf("1) ID. \n");
    printf("2) Date and Slot time. \n");
    printf ("===========================================\n");
    printf("Choose the part of the information that you want to change : ");
    uint8_t choice = ZERO;
    scanf ("%i", &choice);
    switch(choice){
        case EDIT_ID:
            Editing_ID(); /* Calling a helper function to Edit the ID and restore it in the DateBase. */
        break;
        case EDIT_SLOT:
            Editing_Date_and_Time(); /* Calling a helper function to Edit the Slot and restore it in the DateBase. */
        break;
        default :
        printf("No Such a Choice !! , Please Try again \n");
        break;
    }
    
}

/************************************************************************************************************
 * @brief  : Function to Cancel the patient informations and deletes it from the DataBase File.
 * @param  : it takes nothing as a parameter (but it takes the ID from the Admin).
 * @return : No returning.
*************************************************************************************************************/
void Cancel_patient_reservation(void){
    uint8_t flag = 0; /* Flag for how many invalid ID Or Date has been entered the limit is 3.*/
    uint32_t ID = ZERO;  /* Variable to store the ID of the patient who wants to be deleted. */
    uint8_t  Ret_Val = E_NOT_OK; /* variable to return the status of the function through it.  */
    uint32_t ID_line = 1; /* variable to store in it the line which contains the specific ID. */
ID_again:
    printf ("Please enter patient ID : ");
    scanf  ("%i", &ID); /* Getting the ID of the patient. */
    Ret_Val = Compare_IDs(ID); /* Calling a helper function to check if the ID is already exist or not. */
    if (E_OK == Ret_Val){ /* The ID is exist. */
        FILE* l_file ;  /* Creating Pointer to FILE to open the DataBase file to store the Data. */
        uint8_t buffer[130]; /* Character array to store the lines in it. */
        uint8_t ID_Line_Data[130]; /* Character array to store the ID's line in it. */
        l_file = fopen("Data.txt", "r"); /* Openning the file for reading. */ 
        while (fgets(buffer, sizeof(buffer), l_file)){ /* while there is Data in the file this loop will continue. */
            uint32_t l_ID = ZERO;  /* local variable for ID Fetched from the DataBase file. */
            uint8_t Fetched_ID[6]; /* Character array to store the Fetched ID from the DataBase file as a String. */
            strncpy (Fetched_ID, buffer + 4, 5); /* Every line stored in the Buffer array, ID will be fetched from it and stored in Fetched_ID array. */
            l_ID = atoi(Fetched_ID); /* Converting the String ID fetched from the Buffer to integer ID. */
            /* Comparing the ID provided by the Admin and the One which Fetched from the line */
            if(l_ID == ID){ /* If it is our ID then we store this line in ID_Line_Data array to delete it later. */
                strncpy(ID_Line_Data, buffer, 130);
                break;
            }
            ID_line++; /* If IDs are not equal we will increment the Counter. */
        }
        fclose(l_file); /* Closing the File after getting our ID. */
        Deleting_Specific_Line(ID_line); /* Calling a helper function  to delete the Spicific line which we know its number in this variable (ID_line). */
        printf("Done .. Patient with ID [%i] his/her Reservation has been successfuly Canceled. \n", ID);
    }
    else{ /* The ID isn't exist. */
        printf("This Patient does not exist in our Database.\n");
        flag++;
        if (flag != 3){
            goto ID_again;
        }
        else {
            printf("You have reached the Limit.\n");
        }
    }
}

/************************************************************************************************************
 * @brief  : Helper function to check if the ID provided by the Admin is already exist or NOT.
 * @param  : it takes variable (ID) contains the ID.
 * @return : It returns state of the function if the ID is already exist or not.
 *           If the function returns E_NOT_OK -> then the ID isn't exist. 
 *           If the function returns E_OK     -> then the ID is exist.
*************************************************************************************************************/
static uint8_t Compare_IDs(uint32_t ID){
    uint8_t Ret_Val = E_NOT_OK; /* variable to return the status of the function through it.  */
    FILE *l_file = NULL;        /* Creating Pointer to FILE to open the DataBase file to get the ID. */
    uint8_t l_buffer[130];      /* Character array to store the lines in it. */
    l_file = fopen("Data.txt", "r");  /* Openning the file for reading. */ 
    while(fgets (l_buffer, sizeof(l_buffer), l_file)){
        uint32_t l_ID = ZERO;  /* local variable for ID Fetched from the DataBase file. */
        uint8_t Fetched_ID[4]; /* Character array to store the Fetched ID from the DataBase file as a String. */
        strncpy (Fetched_ID, l_buffer + 4, 3); /* Every line stored in the Buffer array, ID will be fetched from it and stored in Fetched_ID array. */
        l_ID = atoi(Fetched_ID); /* Converting the String ID fetched from the Buffer to integer ID. */
        if(l_ID == ID){ /* If the two IDs are equal then the ID is already exist in our DataBase. */
            Ret_Val = E_OK;
            break;
        }
    }
    fclose(l_file); /* Closing the File after getting our ID. */
    return(Ret_Val);
}

/************************************************************************************************************
 * @brief  : Helper function to check if the Slot provided by the Admin is already been taken or NOT.
 * @param  : it takes two character arrays one contains the Date and the other contains Time.
 * @return : It returns state of the function if the ID is already exist or not.
 *           If the function returns E_NOT_OK -> then the Slot hasn't benn taken. 
 *           If the function returns E_OK     -> then the Slot is free.
*************************************************************************************************************/
static uint8_t Compare_Date_and_Slot(const uint8_t Date[], const uint8_t Slot[]){
    uint8_t Ret_Val = E_NOT_OK; /* variable to return the status of the function through it.  */
    FILE *l_file = NULL;        /* Creating Pointer to FILE to open the DataBase file to get the ID. */
    uint8_t l_buffer[130];      /* Character array to store the lines in it. */
    l_file = fopen("Data.txt", "r"); /* Openning the file for reading. */

    if (l_file != NULL) { /* check if the file is opened or not. */
        while (fgets(l_buffer, sizeof(l_buffer), l_file)) {
            uint8_t Fetched_Date[11]; /* Character array to store the Fetched Date to be compared with the Date provided. */
            uint8_t Fetched_Slot[5];  /* Character array to store the Fetched Time to be compared with the Time provided. */

            uint8_t *Date_Founded = strstr(l_buffer, "Date: "); /* Searching in the Featched line for the word "Date". */
            if (Date_Founded != NULL) {
                strncpy(Fetched_Date, Date_Founded + 6, sizeof(Fetched_Date) - 1); /* Fetch the Date and store it in Fetched_Date array. */
                    Fetched_Date[sizeof(Fetched_Date) - 1] = '\0'; /* Putting the null termination '\0'. */

                    uint8_t *Slot_Founded = strstr(l_buffer, "Slot: "); /* Searching in the Featched line for the word "Slot". */
                    if (Slot_Founded != NULL) {
                        strncpy(Fetched_Slot, Slot_Founded + 6, sizeof(Fetched_Slot) - 1); /* Fetch the Time and store it in Fetched_Slot array. */
                        Fetched_Slot[sizeof(Fetched_Slot) - 1] = '\0'; /* Putting the null termination '\0'. */

                        uint8_t temp_arr1[16]; /* Creating an array to combine the Date and Time provided by the Admin to compare them once. */
                        snprintf(temp_arr1, sizeof(temp_arr1), "%s%s", Date, Slot);

                        uint8_t temp_arr2[16]; /* Creating an array to combine the Date and Time Featched from the DataBase to compare them once. */
                        snprintf(temp_arr2, sizeof(temp_arr2), "%s%s", Fetched_Date, Fetched_Slot);
                        /* Compairing the to arrays and if they are matching then the Slot has been taken. */
                        if (!(strcmp(temp_arr1, temp_arr2))) {
                            Ret_Val = E_OK;
                            break;
                        }
                    }
                }
            }
        fclose(l_file); /* Closing the File after getting our Slot Date and Time. */
    }
    return Ret_Val;
}

/************************************************************************************************************
 * @brief  : Helper function to edit the ID and restore it.
 * @param  : it takes nothing as a parameter (but it takes the old and new ID from the Admin).
 * @return : No returning.
*************************************************************************************************************/
static void Editing_ID(void){
    uint8_t flag = 0; /* Flag for how many invalid ID Or Date has been entered the limit is 3.*/
    uint8_t Ret_Val = E_NOT_OK; /* variable to return the status of the function through it. */ 
    uint32_t ID_line = 1;       /* variable to store in it the line which contains the specific ID. */
    uint32_t New_ID = ZERO;     /* variable to store the new ID provided by the Admin. */
    uint32_t Old_ID = ZERO;     /* variable to store the old ID provided by the Admin. */
OldID_again:
    printf ("Enter The old ID : ");
    scanf("%d", &Old_ID); /* Getting the old ID from the Admin. */
    Ret_Val = Compare_IDs(Old_ID); /* Calling a helper function to check if this old ID is exist in our DataBase or NOT. */
    if(Ret_Val == E_OK){ /* it exists. */
        printf("Right .. we had this ID \n");
    NewID_again:
        printf ("Enter The New ID : ");
        scanf("%d", &New_ID); /* Getting the new ID from the Admin. */
        Ret_Val = Compare_IDs(New_ID); /* Calling a helper function to check if this new ID is exist in our DataBase or NOT. */
        if (Ret_Val == E_OK){/* if the New ID is like any of our IDs*/
            printf("This ID [%i] is Already exist \n", New_ID);
            flag++;
            if (flag != 4){
                goto NewID_again;
            }
            else {
                printf("You have reached the Limit.\n");
            }
        }
        else{
            FILE* l_file ; /* Creating Pointer to FILE to open the DataBase file to get the ID. */
            uint8_t buffer[130]; /* Character array to store the lines in it. */
            uint8_t ID_Line_Data[130]; /* Character array to store the ID's line in it. */
            l_file = fopen("Data.txt", "r");  /* Openning the file for reading. */
            while (fgets(buffer, sizeof(buffer), l_file)){
                uint32_t l_ID = ZERO; /* local variable for ID Fetched from the DataBase file. */
                uint8_t Fetched_ID[6]; /* Character array to store the Fetched ID from the DataBase file as a String. */
                strncpy (Fetched_ID, buffer + 4, 5); /* Every line stored in the Buffer array, ID will be fetched from it and stored in Fetched_ID array. */
                l_ID = atoi(Fetched_ID); /* Converting the String ID fetched from the Buffer to integer ID. */
                if(l_ID == Old_ID){ /* If it is our ID then we store this line in ID_Line_Data array to delete it later. */
                    strncpy(ID_Line_Data, buffer, 130);
                    break;
                }
                ID_line++; /* If IDs are not equal we will increment the Counter. */
            }
            fclose(l_file); /* Closing the File after getting our ID. */
            Deleting_Specific_Line(ID_line); /* Calling a helper function to Delete the Line of the old ID. */
            Adding_A_new_Line_Changing_ID(ID_Line_Data, New_ID); /* Calling a helper function to store the Line of the new ID. */
            printf("Done .. The ID has been changed successfully. \n");
        }
    }
    else { /* the old ID dosn't exist. */
        printf("Wrong ID !!\n");
        flag++;
        if (flag != 3){
            goto OldID_again;
        }
        else {
            printf("You have reached the Limit.\n");
        }
    }
 }

/************************************************************************************************************
 * @brief  : Helper function to edit the Slot and restore it.
 * @param  : it takes nothing as a parameter (but it takes the old and new Slot and the patient ID from the Admin).
 * @return : No returning.
*************************************************************************************************************/
static void Editing_Date_and_Time(void){
    uint8_t flag = 0; /* Flag for how many invalid ID Or Date has been entered the limit is 3.*/
    uint8_t Ret_Val = E_NOT_OK; /* variable to return the status of the function through it. */
    uint32_t ID_line = 1;       /* variable to store in it the line which contains the specific ID. */
    uint32_t Patient_ID = ZERO; /* variable to take the ID of the patient from the Admin. */
    uint8_t New_Date[11];       /* character array to store the new date. */
    uint8_t New_Slot[5];        /* character array to store the new Slot. */
ID_again:
    printf("Please enter the patient ID : ");
    scanf ("%i", &Patient_ID);  /* Getting the ID. */
    clearInputBuffer();
    Ret_Val = Compare_IDs(Patient_ID); /* Calling a helper function to check if this ID is exist in our DataBase or NOT. */
    if (Ret_Val != E_OK) { /* No it isn't. */
        printf("The entered ID does not match any existing record.\n");
        flag++;
        if (flag != 3){
            goto ID_again;
        }
        else {
            printf("You have reached the Limit.\n");
        }
    }
    else { /* it is exist. */
        Print_Patient_Record(Patient_ID); /* Calling a helper function to print the patient information of this ID. */
    Date_again:
        printf  ("Please enter the new Date (in this Format \"00/00/0000\") : ");
        fgets   (New_Date, sizeof(New_Date), stdin); /* Getting the new date. */
        New_Date[10] = '\0';
        clearInputBuffer();
        printf  ("Please enter the new Slot (Choose from these [02:00 pm - 02:30 pm - 03:30 pm - 04:00 pm - 04:30 pm] ) : ");
        fgets   (New_Slot, sizeof(New_Slot), stdin); /* Getting the new Slot. */
        New_Slot[4] = '\0';
        clearInputBuffer();
        Ret_Val = Compare_Date_and_Slot(New_Date, New_Slot); /* Calling a helper function to check if the new date and slot are exist before or NOT. */
        if (Ret_Val == E_OK){ /* there are exist. */
            printf("This Slot [Data : %s & Slot = %s0] already taken.\n", New_Date, New_Slot);
            flag++;
            if (flag != 3){
                goto Date_again;
            }
            else {
                printf("You have reached the Limit.\n");
            }
        }
        else { /* there are new. */ 
            FILE* l_file; /* Creating Pointer to FILE to open the DataBase file to get the ID. */
            uint8_t buffer[130]; /* Character array to store the lines in it. */
            uint8_t ID_Line_Data[130]; /* Character array to store the ID's line in it. */
            l_file = fopen("Data.txt", "r"); /* Openning the file for reading. */
            while (fgets(buffer, sizeof(buffer), l_file)){
                uint32_t l_ID = ZERO; /* local variable for ID Fetched from the DataBase file. */
                uint8_t Fetched_ID[6]; /* Character array to store the Fetched ID from the DataBase file as a String. */
                strncpy (Fetched_ID, buffer + 4, 5); /* Every line stored in the Buffer array, ID will be fetched from it and stored in Fetched_ID array. */
                l_ID = atoi(Fetched_ID); /* Converting the String ID fetched from the Buffer to integer ID. */
                if(l_ID == Patient_ID){  /* If it is our ID then we store this line in ID_Line_Data array to delete it later. */
                    strncpy(ID_Line_Data, buffer, 130);
                    break;
                }
                ID_line++; /* If IDs are not equal we will increment the Counter. */
            }
            fclose(l_file); /* Closing the file */
            Deleting_Specific_Line(ID_line); /* Calling a helper function to Delete the Line of the ID. */
            Adding_New_Line_Changing_Date_Time(ID_Line_Data, New_Date, New_Slot);  /* Calling a helper function to store the Line of the new Slot. */
            printf("\nDone .. The new record is ");
            Print_Patient_Record(Patient_ID);  /* Calling a helper function to print the patient information of this ID after editing. */
        }
    }
}

/************************************************************************************************************
 * @brief  : Helper function to edit the Slot and restore it.
 * @param  : it takes the number of the spicific line to be deleted.
 * @return : No returning.
*************************************************************************************************************/
static void Deleting_Specific_Line(uint32_t line){
    FILE *l_file = NULL;  /* Creating Pointer to FILE as a Temporary file. */
    FILE *my_file = NULL; /* Creating Pointer to FILE to open the DataBase file to delete the line. */
	uint32_t l_counter = 1; /* Local counter for the looping. */
	uint8_t Buffer[1000];  /* Character array to store the lines in it. */
	my_file = fopen("Data.txt", "r"); /* Openning the file for reading. */
	l_file  = fopen("Temp.txt", "w"); /* Openning the Temp file for writing. */
    /* while we aren't reach our line take the data in the temp file. */
	while (fgets(Buffer, sizeof(Buffer), my_file)){
		if (l_counter != line){
			fputs(Buffer, l_file);
		}
		l_counter++;
	}
	fclose(my_file); /* closr our file. */
	fclose(l_file);  /* closr Temp file. */
	my_file = fopen("Data.txt", "w"); /* Openning the file for writing. */
	l_file  = fopen("Temp.txt", "r"); /* Openning the Temp file for reading. */
    /* takeing the data from the Temp file without the specific line. */
	while (fgets(Buffer, sizeof(Buffer), l_file)){
        fputs(Buffer, my_file);
	}
	fclose(my_file); /* closr our file. */
	fclose(l_file);  /* closr Temp file. */

	remove("Temp.txt"); /* Deleting the Temp file. */
}

/************************************************************************************************************
 * @brief  : Helper function to swap the old ID with new one and adding the edited Line with the new ID to DataBase.
 * @param  : it takes character array that contains the Fetched line and the new ID.
 * @return : No returning.
*************************************************************************************************************/
static void Adding_A_new_Line_Changing_ID(uint8_t ID_Line_Data[], uint32_t New_ID){
    FILE *fp; /* Creating Pointer to FILE to open the DataBase file to add the modified line. */
    uint8_t ID[5]; /* character array to store the ID as a string after conerting it from integer. */
    itoa(New_ID, ID, 10); /* Converting the intger ID to string and store it in the array. */
    if (New_ID < 10){ /* if the ID is just one digit we will replace it. */
        ID_Line_Data[4] = ID[0]; /* if we had 999 and we need to replace it with 2 after this line it will be 299. */
        ID_Line_Data[5] = ' ';   /* so, we used the following to lines to replace the 99 in 299 to be 2 9. */
        ID_Line_Data[6] = ' ';   /* so, we used the following to lines to replace the 99 in 299 to be 2  . */
    }
    else if (10 <= New_ID && New_ID < 100){ /* if the ID is just two digit we will replace it. */
        ID_Line_Data[4] = ID[0]; /* if we had 999 and we need to replace it with 22 after this line it will be 299. */
        ID_Line_Data[5] = ID[1]; /* if we had 999 and we need to replace it with 22 after this line it will be 229. */
        ID_Line_Data[6] = ' ';   /* so, we used the following to lines to replace the 99 in 299 to be 22 . */
    }
    else if (New_ID >= 100){ /* if the ID is three digit we will replace it. */
        ID_Line_Data[4] = ID[0]; 
        ID_Line_Data[5] = ID[1];
        ID_Line_Data[6] = ID[2];
    }
    else { /* Nothing */ }
    fp = fopen("Data.txt", "a+"); /* Openning the file to put the modified line with the new ID. */
    fputs(ID_Line_Data, fp);
    fclose(fp); /* Closing the file. */
}

/************************************************************************************************************
 * @brief  : Helper function to swap the old Slot with new one and adding the edited Line with the new Slot to DataBase.
 * @param  : it takes character array that contains the Fetched line and the new Date and Time.
 * @return : No returning.
*************************************************************************************************************/
static void Adding_New_Line_Changing_Date_Time(uint8_t ID_Line_Data[], uint8_t Date[], uint8_t Slot[]){
    uint32_t l_counter = ZERO;  /* Local counter for the looping. */
    FILE *fp; /* Creating Pointer to FILE to open the DataBase file to add the modified line. */

    while (Date[l_counter]){ /* Coping the new date and overwrite it on the old one. */
        ID_Line_Data[16 + l_counter] = Date[l_counter];
        l_counter++;
    }
    /* Coping the new time and overwrite it on the old one. */
    ID_Line_Data[35] = Slot[0];
    ID_Line_Data[36] = Slot[1];
    ID_Line_Data[37] = Slot[2];
    ID_Line_Data[38] = Slot[3];

    fp = fopen("Data.txt", "a+"); /* Openning the file to put the modified line with the new Slot. */
    fputs(ID_Line_Data, fp);
    fclose(fp); /* Closing the file. */
}

/************************************************************************************************************
 * @brief  : Helper function to print the patient record.
 * @param  : it takes patient ID.
 * @return : No returning.
*************************************************************************************************************/
static void Print_Patient_Record(uint32_t ID){
    FILE* my_file = NULL; /* Creating Pointer to FILE to open the DataBase file to Fetch the needed line. */
    uint8_t Buffer[150];  /* Character array to store the lines in it. */
    uint8_t ID_Line_Data[150]; /* Character array to store the specific line in it. */
    /* Character arrays to store the patient record in it. */
    uint8_t Fetched_ID[6], Fetched_Date[11], Fetched_Slot[6], Fetched_Age[3], Fetched_Gender[2], Fetched_Name[51];
    my_file = fopen("Data.txt","r"); /* Openning the file to read the record. */
    while(fgets(Buffer, sizeof(Buffer), my_file)){
        uint32_t l_ID = ZERO;  /* local variable for ID Fetched from the DataBase file. */
        uint8_t Fetched_ID[6]; /* Character array to store the Fetched ID from the DataBase file as a String. */
        strncpy (Fetched_ID, Buffer + 4, 5); /* Every line stored in the Buffer array, ID will be fetched from it and stored in Fetched_ID array. */
        l_ID = atoi(Fetched_ID); /* Converting the String ID fetched from the Buffer to integer ID. */
        if(l_ID == ID){ /* If it is our ID then we store this line stores the patient record. */
            strncpy(ID_Line_Data, Buffer, 150);
            break;
        }
    }
    fclose(my_file); /* Closing the File. */
    strncpy(Fetched_ID, ID_Line_Data + 4, 3);       /* coping the ID from the array that contains the patient record. */
    Fetched_ID[sizeof(Fetched_ID) - 1] = '\0';
    strncpy(Fetched_Date, ID_Line_Data + 16, 10);   /* coping the Date from the array that contains the patient record. */
    Fetched_Date[sizeof(Fetched_Date) - 1] = '\0';
    strncpy(Fetched_Slot, ID_Line_Data + 35, 5);    /* coping the Time from the array that contains the patient record. */
    Fetched_Slot[sizeof(Fetched_Slot) - 1] = '\0';
    strncpy(Fetched_Age, ID_Line_Data + 48, 2);     /* coping the Age from the array that contains the patient record. */
    Fetched_Age[sizeof(Fetched_Age) - 1] = '\0';
    char *ptr_gender = strstr(ID_Line_Data, "Gender: "); /* Searching in the Featched line for the word "Gender". */
    ptr_gender += 8; /* Getting the pointer to point to the start of the gender. */
    strncpy(Fetched_Gender, ptr_gender, sizeof(Fetched_Gender) - 1); /* coping the Gender from the array that contains the patient record. */
    char *ptr_name = strstr(ID_Line_Data, "Name: "); /* Searching in the Featched line for the word "Name". */
    ptr_name += 6; /* Getting the pointer to point to the start of the name. */
    strncpy(Fetched_Name, ptr_name, sizeof(Fetched_Name) - 1); /* coping the Name from the array that contains the patient record. */
    /* Printing the patient Record. */
    printf("\n===========================================\n");
    printf(" Patient ID     : %i\n", atoi(Fetched_ID));
    printf(" Patient Name   : %s"  , Fetched_Name);
    printf(" Patient Age    : %i\n", atoi(Fetched_Age));
    printf(" Patient Gender : %s\n", Fetched_Gender);
    printf(" Examine Date   : %s\n", Fetched_Date);
    printf(" Examine Slot   : %s\n", Fetched_Slot);
    printf("===========================================\n");
}

/************************************************************************************************************
 * @brief  : Helper function to clear the input buffer.
 * @param  : it takes nothing.
 * @return : No returning.
*************************************************************************************************************/
static inline void clearInputBuffer(void) {
   uint32_t c;
   while ((c = getchar()) != '\n' && c != EOF); /* Blocking line until the input is new line or end of file to put this char 
                                                    '\n' or enter ASCI in variable (c) to clear the buffer. */
}
