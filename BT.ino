/* Funciones utilizadas para la comunicación serial - bluetooh  */

//============

void recvWithStartEndMarkers() 
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (BT.available() > 0 && newData == false) 
    {
        rc = BT.read();

        if (recvInProgress == true) 
        {
            if (rc != endMarker) 
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) 
                {
                    ndx = numChars - 1;
                }
            }
            else 
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) 
        {
            recvInProgress = true;
        }
    }
}

//============

void parseData() 
{      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string

    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    doubleFromPC1 = strtod(strtokIndx, NULL);     // convert this part to a double

    strtokIndx = strtok(NULL, ",");
    doubleFromPC2 = strtod(strtokIndx, NULL);     // convert this part to a double

    strtokIndx = strtok(NULL, ",");
    doubleFromPC3 = strtod(strtokIndx, NULL);     // convert this part to a double

    strtokIndx = strtok(NULL, ",");
    doubleFromPC4 = strtod(strtokIndx, NULL);     // convert this part to a double

    strtokIndx = strtok(NULL, ",");
    doubleFromPC5 = strtod(strtokIndx, NULL);     // convert this part to a double

    strtokIndx = strtok(NULL, ",");
    doubleFromPC6 = strtod(strtokIndx, NULL);     // convert this part to a double

}

//============

void assignData() 
{   
    Vel_mapeo = integerFromPC;
    Kp_map = doubleFromPC1;
    Ki_map = doubleFromPC2;
    Kd_map = doubleFromPC3;

    BT.print("Vel: ");
    BT.print(Vel_mapeo);
    BT.print(" Kp: ");
    BT.printf("%lf", Kp_map);
    BT.print(" Ki: ");
    BT.printf("%lf", Ki_map);
    BT.print(" Kd: ");
    BT.printf("%lf", Kd_map);
    BT.println();
}
