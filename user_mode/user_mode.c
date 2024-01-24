/************************************************************************************************************
 * @file   : user_mode.c.
 * @author : Ahmed Magdy Gad.
 * @link   : https://www.linkedin.com/in/ahmed-gad-05b296272/ .
*************************************************************************************************************/

#include "user_mode.h"

/* The static Helper Functions prototypes used by this file. */
/************************************************************************************************************/
static void Print_Reservations(const User_mode_t* Reservation_Info, uint32_t Len);
static void Get_Reservation_Details(uint8_t arr[], User_mode_t* Reservation_Info, uint32_t Index);
static uint8_t Compare_IDs(uint32_t ID);
static uint8_t Compare_Date(const uint8_t Date[]);
/************************************************************************************************************/

/************************************************************************************************************
 * @brief  : Function to print the patient record.
 * @param  : it takes nothing but requires the ID from the User.
 * @return : No returning.
*************************************************************************************************************/
void Print_Patient_Record(void){
    uint8_t flag = 0; /* Flag for how many invalid ID Or Date has been entered the limit is 3.*/
    uint8_t Ret_Val = E_NOT_OK;
    uint32_t ID = ZERO; /* Variable to store the ID. */
ID_again:
    printf("Enter the patient ID : ");
    scanf ("%i", &ID); /* Getting the ID from the user. */
    Ret_Val = Compare_IDs(ID);
    if (Ret_Val == E_NOT_OK){
        printf("Sorry .. this ID [%i] isn't exist \n", ID);
        flag++;
        if (flag != 3){
            goto ID_again;
        }
        else {
            printf("You have reached the Limit.\n");
        }
    }
    else {
        FILE* my_file = NULL; /* Creating Pointer to FILE to open the DataBase file to Fetch the needed line. */
        uint8_t Buffer[150];  /* Character array to store the lines in it. */
        uint8_t ID_Line_Data[150]; /* Character array to store the specific line in it. */
        /* Character arrays to store the patient record in it. */
        uint8_t Fetched_ID[6], Fetched_Date[11], Fetched_Slot[6], Fetched_Age[3], Fetched_Gender[2], Fetched_Name[51];
        my_file = fopen("Data.txt","r"); /* Openning the file to read the record. */
        while(fgets(Buffer, sizeof(Buffer), my_file)){
            uint32_t l_ID = ZERO; /* local variable for ID Fetched from the DataBase file. */
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
        Fetched_Gender[sizeof(Fetched_Gender) - 1] = '\0';
        char *ptr_name = strstr(ID_Line_Data, "Name: "); /* Searching in the Featched line for the word "Name". */
        ptr_name += 6; /* Getting the pointer to point to the start of the name. */
        strncpy(Fetched_Name, ptr_name, sizeof(Fetched_Name) - 1); /* coping the Name from the array that contains the patient record. */
        /*** Printing the patient Record. ***/
        printf("\n===========================================\n");
        printf(" Patient ID     : %i\n", atoi(Fetched_ID));
        printf(" Patient Name   : %s"  , Fetched_Name);
        printf(" Patient Age    : %i\n", atoi(Fetched_Age));
        printf(" Patient Gender : %s\n", Fetched_Gender);
        printf(" Examine Date   : %s\n", Fetched_Date);
        printf(" Examine Slot   : %s\n", Fetched_Slot);
        printf("===========================================\n");
    }
}

/************************************************************************************************************
 * @brief  : Function to print the patient records on a specific day.
 * @param  : it takes nothing but requires the Date from the User.
 * @return : No returning.
*************************************************************************************************************/
void View_All_Recervations_Of_Specific_Day(void){
    uint8_t Ret_Val = E_NOT_OK;
    User_mode_t Reservation_Info[5]; /* array of 5 structs ,, why 5? cause every day the clinic has just 5 appointments. */
    uint32_t Flag = ZERO;       /* Flag to indicates how many records we have. */
    uint32_t l_Index = ZERO;    /* variable to be passed to a helper function to use it as Index to the array of the structs. */
    uint8_t l_Date[11];         /* array of characters to store the date in it. */
    uint8_t Buffer[130];        /* Character array to store the lines in it. */
    printf("Enter the date you want to view reservations for (in this format \"00/00/0000\") : ");
    fgets (l_Date, sizeof(l_Date), stdin); /* Getting the Date. */
    //l_Date[strcspn(l_Date, "\n")] = '\0';
    l_Date[sizeof(l_Date) - 1] = '\0';
    Ret_Val = Compare_Date(l_Date);
    if (Ret_Val == E_NOT_OK){
        printf("Sorry .. in this day we haven't any reservations :(. \n");
    }
    else {
        FILE *my_file; /* Creating Pointer to FILE to open the DataBase file to Fetch the needed line. */
        my_file = fopen("Data.txt","r"); /* Openning the file to reading. */
        while(fgets(Buffer, sizeof Buffer, my_file)){
            uint8_t Fetched_Date[11]; /* array of characters to store the Fetched Date taken from the DataBase. */
            uint8_t *Date_Founded = strstr(Buffer, "Date: "); /* Searching in the Featched line for the word "Date". */
            strncpy(Fetched_Date, Date_Founded + 6, sizeof(Fetched_Date) - 1); /* Fetch the Date and store it in Fetched_Date array. */
            Fetched_Date[sizeof(Fetched_Date) - 1] = '\0';
            if (strcmp(l_Date, Fetched_Date) == ZERO){ /* Compareing the two Dates. */
                Get_Reservation_Details(Buffer, Reservation_Info, l_Index); /* Getting a helper function to store the info in each struct. */
                Flag++; 
                l_Index++;
            }
            else {
                continue;
            }
        }
        fclose(my_file); /* Closing the file. */
        if (Flag == ZERO){ /* if the Flag is ZERO then there is no records. */
            printf("\nNo record found on that day.\n");
        }
        else { /* if Flag != ZERO the we have a Flag Number of records. */
            printf("\nNumber of records found on that day : %d and they are ...", Flag);
            Print_Reservations(Reservation_Info, Flag); /* calling a helper function to print all the records in that day. */
        }
        }
}

/************************************************************************************************************
 * @brief  : Helper function to get the Details of the the patients in this day.
 * @param  : it takes the array which contains the line which had the specific day and pointer to the array of structs
 *           and the Index to select the elements on the array.
 * @return : No returning.
*************************************************************************************************************/
static void Get_Reservation_Details(uint8_t arr[], User_mode_t *Reservation_Info, uint32_t Index){
    /* Character arrays to store the patient record in it. */
    uint8_t l_Fetched_Date[11], l_Fetched_Slot[6], l_Fetched_ID[6];
    strncpy(l_Fetched_Date, arr + 16, 10); /* Fetching the Date. */
    l_Fetched_Date[sizeof(l_Fetched_Date) - 1] = '\0';
    strncpy (l_Fetched_Slot, arr + 35, 4); /* Fetching the Slot. */
    l_Fetched_Slot[5] = '\0';
    strcpy  (Reservation_Info[Index].Date, l_Fetched_Date); /* Storing the Date in the struct. */
    strcpy  (Reservation_Info[Index].Slot, l_Fetched_Slot); /* Storing the Slot in the struct. */
    strncpy (l_Fetched_ID, arr + 4, 3); /* Fetching the ID. */
    l_Fetched_ID[4] = '\0';
    Reservation_Info[Index].ID = atoi(l_Fetched_ID);        /* Storing the ID in the struct. */
}

/************************************************************************************************************
 * @brief  : Function to print the patient records on a specific day.
 * @param  : it takes the array of structs and the Index to select the elements on the array.
 * @return : No returning.
*************************************************************************************************************/
static void Print_Reservations(const User_mode_t* Reservation_Info, uint32_t Len){
    uint32_t l_counter = ZERO; /* Local counter for the looping. */
    for (l_counter = ZERO; l_counter < Len; l_counter++){
        /* Printing the data. */
        printf("\n===========================================\n");
        printf(" Patient ID     : %i \n", Reservation_Info[l_counter].ID);
        printf(" Examine Date   : %s \n", Reservation_Info[l_counter].Date);
        printf(" Examine Slot   : %s0\n", Reservation_Info[l_counter].Slot);
        printf("===========================================\n");
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
static uint8_t Compare_Date(const uint8_t Date[]){
    uint8_t Ret_Val = E_NOT_OK; /* variable to return the status of the function through it.  */
    FILE *l_file = NULL;        /* Creating Pointer to FILE to open the DataBase file to get the ID. */
    uint8_t l_buffer[130];      /* Character array to store the lines in it. */
    l_file = fopen("Data.txt", "r"); /* Openning the file for reading. */

    if (l_file != NULL) { /* check if the file is opened or not. */
        while (fgets(l_buffer, sizeof(l_buffer), l_file)) {
            uint8_t Fetched_Date[11]; /* Character array to store the Fetched Date to be compared with the Date provided. */
            uint8_t *Date_Founded = strstr(l_buffer, "Date: "); /* Searching in the Featched line for the word "Date". */
            if (Date_Founded != NULL) {
                strncpy(Fetched_Date, Date_Founded + 6, sizeof(Fetched_Date) - 1); /* Fetch the Date and store it in Fetched_Date array. */
                    Fetched_Date[sizeof(Fetched_Date) - 1] = '\0'; /* Putting the null termination '\0'. */
                    /* Compairing the to arrays and if they are matching then the Slot has been taken. */
                    if (!(strcmp(Date, Fetched_Date))) {
                        Ret_Val = E_OK;
                        break;
                    }
                }
                
            }
        fclose(l_file); /* Closing the File after getting our Slot Date and Time. */
    }
    return Ret_Val;
}