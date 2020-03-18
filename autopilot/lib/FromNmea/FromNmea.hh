#ifndef FROMNMEA_H
#define FROMNMEA_H



void errorHandler()
{
    Serial.print("*** Error : ");
    Serial.println(parser.error());
}

void autoPilotCmd_Handler()         //$NRPAP,41,1,FE*58\r\n";
{
    parser.withMsg=4;

    parser.bufSeatalk[0]=0x186;
    parser.bufSeatalk[1]=0x41;
    // Serial.print("Got RPPAP with ");
    // Serial.print(parser.argCount());
    // Serial.println(" arguments");
    char arg0[3];
    char arg1[3];
    char arg2[3];
    if (parser.getArg(0, arg0))
        Serial.print(arg0);
    Serial.print(">");    
    if (parser.getArg(1, arg1))
        Serial.print(arg1);
    Serial.print(">");
    if (parser.getArg(2, arg2))
        Serial.print(arg2);
    Serial.println();

    long l = strtol(arg1, NULL, 16);
    parser.bufSeatalk[2] = (uint16_t)l;
    l = strtol(arg2, NULL, 16);
    parser.bufSeatalk[3]= (uint16_t)l;

    Serial.print(parser.bufSeatalk[2]);
    Serial.print("++");
    Serial.print(parser.bufSeatalk[3]);
    Serial.println();
}

void apparentWind_Handler()      //    "$IIMWV,339,R,09.52,N,A*24\r\n";
{
    parser.bufSeatalk[0]=0x110; //  angle
    parser.bufSeatalk[1]=0x01;

    parser.bufSeatalk[4]=0x111; //  speed
    parser.bufSeatalk[5]=0x01;


    // Serial.print("Got $IIMWV with ");
    // Serial.print(parser.argCount());
    // Serial.println(" arguments");
    int arg0;
    char arg1;
    float arg2;
    char arg3;
    char arg4;
    

    if (parser.getArg(0, arg0))
        Serial.println(arg0);
    Serial.print(">");
    if (parser.getArg(1, arg1))
        Serial.println(arg1);
    Serial.print(">");
    if (parser.getArg(2, arg2))
        Serial.println(arg2);
    Serial.print(">");
    if (parser.getArg(3, arg3))
        Serial.println(arg3);
    Serial.print(">");
    if (parser.getArg(4, arg4))
        Serial.println(arg4);
    Serial.println();

    uint16_t ang = arg0*2;
    uint8_t speed = arg2;
    uint8_t frac = (arg2 - (uint16_t)arg2) * 10;
    Serial.println(ang);
    Serial.println(speed);
    Serial.println(frac);
    parser.bufSeatalk[2]=(uint8_t)(ang>>8); //  speed
    parser.bufSeatalk[3]=(uint8_t)ang;

    parser.bufSeatalk[6]=(uint8_t)speed; //  speed
    parser.bufSeatalk[7]=(uint8_t)frac;
    parser.withMsg=8;
}

#endif // !FROMNMEA_H
