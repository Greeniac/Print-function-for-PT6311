
#define VFD_data 8// If 0 write LCD, if 1 read of LCD
#define VFD_clk 9 // if 0 is a command, if 1 is a data0
#define VFD_ce 10 // OR SS / CS / STB   /// Must be pulsed to LCD fetch data of bus 

  //    Numbers design      
  unsigned char number0[]={(0b11001100),(0b11001100),(0b00000000)}; // 0 unsigned char number0[]={(0b10001000),(0b10000001),(0b00000011)}; // 0
  unsigned char number1[]={(0b10000000),(0b01000000),(0b00000000)}; // 1
  unsigned char number2[]={(0b10000100),(0b10000111),(0b00000000)}; // 2
  unsigned char number3[]={(0b10000100),(0b11000011),(0b00000000)}; // 3
  unsigned char number4[]={(0b10001000),(0b01000011),(0b00000000)}; // 4
  unsigned char number5[]={(0b00001100),(0b11000011),(0b00000000)}; // 5
  unsigned char number6[]={(0b00001100),(0b11000111),(0b00000000)}; // 6
  unsigned char number7[]={(0b10000100),(0b01000000),(0b00000000)}; // 7
  unsigned char number8[]={(0b10001100),(0b11000111),(0b00000000)}; // 8
  unsigned char number9[]={(0b10001100),(0b11000011),(0b00000000)}; // 9
  //    Letters design         
  unsigned char letterA[]={(0b10001100),(0b01000111),(0b00000000)}; // A
  unsigned char letterB[]={(0b10100100),(0b11010010),(0b00000000)}; // B
  unsigned char letterC[]={(0b00001100),(0b10000100),(0b00000000)}; // C
  unsigned char letterD[]={(0b10100100),(0b11010000),(0b00000000)}; // D
  unsigned char letterE[]={(0b00001100),(0b10000101),(0b00000000)}; // E
  unsigned char letterF[]={(0b00001100),(0b00000101),(0b00000000)}; // F
  unsigned char letterG[]={(0b00001100),(0b11000110),(0b00000000)}; // G
  unsigned char letterH[]={(0b10001000),(0b01000111),(0b00000000)}; // H
  unsigned char letterI[]={(0b00100100),(0b10010000),(0b00000000)}; // I
  unsigned char letterJ[]={(0b10000000),(0b11000100),(0b00000000)}; // J
  unsigned char letterK[]={(0b01001000),(0b00100101),(0b00000000)}; // k
  unsigned char letterL[]={(0b00001000),(0b10000100),(0b00000000)}; // L
  unsigned char letterM[]={(0b11011000),(0b01000100),(0b00000000)}; // M
  unsigned char letterN[]={(0b10011000),(0b01100100),(0b00000000)}; // N
  unsigned char letterO[]={(0b10001100),(0b11000100),(0b00000000)}; // O
  unsigned char letterP[]={(0b10001100),(0b00000111),(0b00000000)}; // P
  unsigned char letterQ[]={(0b10001100),(0b11100100),(0b00000000)}; // Q
  unsigned char letterR[]={(0b10001100),(0b00100111),(0b00000000)}; // R
  unsigned char letterS[]={(0b00001100),(0b11000011),(0b00000000)}; // S
  unsigned char letterT[]={(0b00100100),(0b00010000),(0b00000000)}; // T
  unsigned char letterU[]={(0b10001000),(0b11000100),(0b00000000)}; // U
  unsigned char letterV[]={(0b01001000),(0b00001100),(0b00000000)}; // V
  unsigned char letterW[]={(0b10001000),(0b01101100),(0b00000000)}; // W
  unsigned char letterX[]={(0b01010000),(0b00101000),(0b00000000)}; // X
  unsigned char letterY[]={(0b01010000),(0b00010000),(0b00000000)}; // Y
  unsigned char letterZ[]={(0b01000100),(0b10001000),(0b00000000)}; // Z
  unsigned char letterSpace[]={(0b00000000),(0b00000000),(0b00000000)}; // Space
  unsigned char letterAllOn[]={(0b11111111),(0b11111111),(0b00000011)}; // All 17 segments ON

void pt6311_init(void){
  delayMicroseconds(200); //power_up delay
  // Note: Allways the first byte in the input data after the STB go to LOW is interpret as command!!!

  // Configure VFD display (grids)
  cmd_with_stb(0b00001001);//  (0b01000000)    cmd1 14 Grids and 14 segments??? (IC da SGNEC ref: S0791 que é igual 6311 pinout)
  delayMicroseconds(1);
  cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
  delayMicroseconds(1);
  // turn vfd on, stop key scannig
   cmd_with_stb(0b10001000);//(BIN(01100110)); 
  delayMicroseconds(1);
  // Write to memory display, increment address, normal operation
  cmd_with_stb(0b01000000);//(BIN(01000000));
  delayMicroseconds(1);
  // Address 00H - 15H ( total of 11*2Bytes=176 Bits)
  cmd_with_stb(0b11000000);//(BIN(01100110)); 
  delayMicroseconds(1);
  // set DIMM/PWM to value
  cmd_with_stb((0b10001000) | 7);//0 min - 7 max  )(0b01010000)
  delayMicroseconds(1);
}
void send_char(unsigned char grid, unsigned char byte0, unsigned char byte1,unsigned char byte2){
        cmd_with_stb(0b00001001); //       cmd 1 // 10 Grids & 17 Segments
        delayMicroseconds(2);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(2);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Auto increment
        digitalWrite(VFD_ce, LOW);
        delayMicroseconds(2);
            cmd_without_stb((0b11000000) | grid); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(byte0); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(byte1); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(byte2); // only half byte of third byte.
            //
            digitalWrite(VFD_ce, HIGH);
            delayMicroseconds(2);
}
void cmd_4bitsWithout_stb(unsigned char a){
  // send without stb
  unsigned char data = 0x00; //value to transmit
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    delayMicroseconds(2);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(2);
   }
}
void cmd_without_stb(unsigned char a){
  // send without stb
  unsigned char data = 0x00; //value to transmit
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    delayMicroseconds(2);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(2);
   }
}
void cmd_4bitsWith_stb(unsigned char a){
  // send with stb
  unsigned char data = 0x00; //value to transmit
  unsigned char mask = 1; //our bitmask
  
  data=a; 
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_ce, LOW);
  delayMicroseconds(1);
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_ce, HIGH);
   delayMicroseconds(1);
}
void cmd_with_stb(unsigned char a){
  // send with stb
  unsigned char data = 0x00; //value to transmit
  unsigned char mask = 1; //our bitmask
  
  data=a; 
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_ce, LOW);
  delayMicroseconds(1);
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_ce, HIGH);
   delayMicroseconds(1);
}
void test_VFD(void){
    clear_VFD();
        
        digitalWrite(VFD_ce, LOW);
        delayMicroseconds(1);
        cmd_with_stb(0b00001001); // cmd 1 // 10 Grids & 17 Segments
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        cmd_with_stb(0b01000000); // cmd 2 //Write VFD, Normal operation; Set pulse as 1/16, Auto increment
        cmd_with_stb(0b10001000 | 0x07); // cmd 2 //set on, dimmer to max
          digitalWrite(VFD_ce, LOW);
          delayMicroseconds(1);
          cmd_without_stb((0b11000000)); //cmd 3 wich define the start address (00H to 15H)
          // Only here must change the bit to test, first 2 bytes and 1/2 byte of third.
          for (int i = 0; i < 8 ; i++){ // test base to 16 segm and 5 grids
            // Zone of test, if write 1 on any position of 3 bytes below position, will bright segment corresponding it.
          cmd_without_stb(0b00000000); // Data to fill table 5*16 = 80 bits
          cmd_without_stb(0b00000000); // Data to fill table 5*16 = 80 bits
          cmd_4bitsWithout_stb(0b00000000); // Data to fill table 5*16 = 80 bits
          }
        digitalWrite(VFD_ce, HIGH);
        delay(1);
        delay(3000);  
}
void setup() {

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);

  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_data, OUTPUT);
  pinMode(VFD_ce, OUTPUT);

  pt6311_init();
 vfdprint("DINGUS");
}
void clear_VFD(void){
  int sum=0;
  /*
  Here I clean all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 16; n++){  //
        cmd_with_stb(0b00001001); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_ce, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(0b00000000); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b00000000); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.
            //
            digitalWrite(VFD_ce, HIGH);
            delayMicroseconds(2);
            sum=(sum +3);
     }
}
void led_on(void){
  byte myByte;
    digitalWrite(VFD_ce, LOW);
    delayMicroseconds(20);
    cmd_without_stb(0b01000001);
    delayMicroseconds(2);
    myByte =(0b00000000);  // Here is only to invert bit of led 1.
    cmd_without_stb(myByte);
    delayMicroseconds(2);
    digitalWrite(VFD_ce, HIGH);
    delayMicroseconds(2);
}
void led_off(void){
  byte myByte;
    digitalWrite(VFD_ce, LOW);
    delayMicroseconds(20);
    cmd_without_stb(0b01000001);
    delayMicroseconds(2);
    myByte =(0b00000010);  // Here is only to invert bit of led 1.
    cmd_without_stb(myByte);
    delayMicroseconds(2);
    digitalWrite(VFD_ce, HIGH);
    delayMicroseconds(2);
}
void all_ON(void){
  int sum=0;
  /*
  Here I test all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 16; n++){  //
        cmd_with_stb(0b00001001); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001010) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_ce, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(0b11111111); // only half byte of third byte.
            //
            digitalWrite(VFD_ce, HIGH);
            delayMicroseconds(3000);
            Serial.println(((0b11000000) | sum), BIN);
            sum=(sum +3);
     }
}
void segBySeg(void){
  int u=0;
  int sum=0;
  unsigned char word1, word2,word3,word4;
  unsigned int mask0=0b0000000000000001;
  unsigned int mask1=0b0000000000000001;
 
      for ( u =0; u < 32; u++){
        cmd_with_stb(0b00001001); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_ce, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            
        if (u<=15){
       word4 = (mask0 & 0xff000000) >>  24; // remaning segments 
       word3 = (mask0 & 0x00ff0000) >>  16; // remaning segments
       word2 = (mask0 & 0x0000ff00) >>  8; // grid 9 & 10 and segments
       word1 = (mask0 & 0x000000ff) >>  0; // grids
        mask0=(mask0 <<= 1);
        }
        if (u>15){
       word4 = (mask1 & 0xff000000) >>  24; // remaning segments 
       word3 = (mask1 & 0x00ff0000) >>  16; // remaning segments
       word2 = (mask1 & 0x0000ff00) >>  8; // grid 9 & 10 and segments
       word1 = (mask1 & 0x000000ff) >>  0; // grids
        mask1=(mask1 <<= 1);
        }
       //
       //Serial.println(mask0, BIN);
       //Serial.println(mask1, BIN);
            cmd_without_stb(word1); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(word2); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(word3); // only half byte of third byte.
            //
            digitalWrite(VFD_ce, HIGH);
            delay(500);
            //Serial.println(((0b11000000) | sum), BIN);
            sum=(sum +3);
    }
}
void msgHiFolks(){ 
  send_char(0x03,(0b00000000), (0b00000000), (0b00000000)); //This grid stay empty!
  send_char(0x06,(0b00001000), (0b10001011), (0b00000010)); //S
  send_char(0x09,(0b10010000), (0b10010010), (0b00000000)); //K
  send_char(0x0C,(0b10001000), (0b10000000), (0b00000000)); //L
  send_char(0x0F,(0b10001000), (0b10000001), (0b00000011)); //O
  send_char(0x12,(0b10000000), (0b10001010), (0b00000010)); //F
  send_char(0x15,(0b00000000), (0b00000000), (0b00000000)); //
  send_char(0x18,(0b00101000), (0b00100000), (0b00000010)); //I
  send_char(0x1B,(0b10000100), (0b10001011), (0b00000001)); //H
}
void characters(){
  clear_VFD();
  send_char(0x03,letterA[0], letterA[1], letterA[2]);
  send_char(0x06,letterB[0], letterB[1], letterB[2]);
  send_char(0x09,letterC[0], letterC[1], letterC[2]);
  send_char(0x0C,letterD[0], letterD[1], letterD[2]);
  send_char(0x0F,letterE[0], letterE[1], letterE[2]);
  send_char(0x12,letterF[0], letterF[1], letterF[2]);
  send_char(0x15,letterG[0], letterG[1], letterG[2]);
  send_char(0x18,letterH[0], letterH[1], letterH[2]);
    delay(2000);
  clear_VFD();
  send_char(0x03,letterI[0], letterI[1], letterI[2]);
  send_char(0x06,letterJ[0], letterJ[1], letterJ[2]);
  send_char(0x09,letterK[0], letterK[1], letterK[2]);
  send_char(0x0C,letterL[0], letterL[1], letterL[2]);
  send_char(0x0F,letterM[0], letterM[1], letterM[2]);
  send_char(0x12,letterN[0], letterN[1], letterN[2]);
  send_char(0x15,letterO[0], letterO[1], letterO[2]);
  send_char(0x18,letterP[0], letterP[1], letterP[2]);
    delay(2000);
  clear_VFD();
  send_char(0x03,letterQ[0], letterQ[1], letterQ[2]);
  send_char(0x06,letterR[0], letterR[1], letterR[2]);
  send_char(0x09,letterS[0], letterS[1], letterS[2]);
  send_char(0x0C,letterT[0], letterT[1], letterT[2]);
  send_char(0x0F,letterU[0], letterU[1], letterU[2]);
  send_char(0x12,letterV[0], letterV[1], letterV[2]);
  send_char(0x15,letterW[0], letterW[1], letterW[2]);
  send_char(0x18,letterX[0], letterX[1], letterX[2]);
      delay(2000);
  clear_VFD();
  send_char(0x03,letterY[0], letterY[1], letterY[2]);
  send_char(0x06,letterZ[0], letterZ[1], letterZ[2]);
  send_char(0x1B,letterSpace[0], letterSpace[1], letterSpace[2]);
      delay(2000);
      clear_VFD();
  send_char(0x03,number0[0], number0[1], number0[2]);
  send_char(0x06,number1[0], number1[1], number1[2]);
  send_char(0x09,number2[0], number2[1], number2[2]);
  send_char(0x0C,number3[0], number3[1], number3[2]);
  send_char(0x0F,number4[0], number4[1], number4[2]);
  send_char(0x12,number5[0], number5[1], number5[2]);
  send_char(0x15,number6[0], number6[1], number6[2]);
  send_char(0x18,number7[0], number7[1], number7[2]);
  delay(2000);
        clear_VFD();
  send_char(0x03,number8[0], number8[1], number8[2]);
  send_char(0x06,number9[0], number9[1], number9[2]);

}
unsigned char convert(char in,int num){
  switch(in){
    case 'A':
    return letterA[num];
  break;
    case 'B':
    return letterB[num];
  break;
    case 'C':
    return letterC[num];
  break;
    case 'D':
    return letterD[num];
  break;
    case 'E':
    return letterE[num];
  break;
    case 'F':
    return letterF[num];
  break;
    case 'G':
    return letterG[num];
  break;
    case 'H':
    return letterH[num];
  break;
    case 'I':
    return letterI[num];
  break;
    case 'J':
    return letterJ[num];
  break;
    case 'K':
    return letterK[num];
  break;
    case 'L':
    return letterL[num];
  break;
    case 'M':
    return letterM[num];
  break;
      case 'N':
    return letterN[num];
  break;
    case 'O':
    return letterO[num];
  break;
    case 'P':
    return letterP[num];
  break;
    case 'Q':
    return letterQ[num];
  break;
    case 'R':
    return letterR[num];
  break;
    case 'S':
    return letterS[num];
  break;
    case 'T':
    return letterT[num];
  break;
    case 'U':
    return letterU[num];
  break;
    case 'V':
    return letterV[num];
  break;
    case 'W':
    return letterW[num];
  break;
    case 'X':
    return letterX[num];
  break;
    case 'Y':
    return letterW[num];
  break;
    case 'Z':
    return letterZ[num];
  break;
    case '0':
    return number0[num];
  break;
      case '1':
    return number1[num];
  break;
      case '2':
    return number2[num];
  break;
      case '3':
    return number3[num];
  break;
      case '4':
    return number4[num];
  break;
      case '5':
    return number5[num];
  break;
      case '6':
    return number6[num];
  break;
      case '8':
    return number8[num];
  break;
        case '9':
    return number9[num];
  break;
        case '7':
    return number7[num];
  break;
  case ' ':
  return letterSpace[num];
  break;
  case NULL:
   return letterSpace[num];
  break;
  }
}
void numbers(){
  clear_VFD();  
  send_char(0x03,number0[0], number0[1], number0[2]);
  send_char(0x06,number1[0], number1[1], number1[2]);
  send_char(0x09,number2[0], number2[1], number2[2]);
  send_char(0x0C,number3[0], number3[1], number3[2]);
  send_char(0x0F,number4[0], number4[1], number4[2]);
  send_char(0x12,number5[0], number5[1], number5[2]);
  send_char(0x15,number6[0], number6[1], number6[2]);
  send_char(0x18,number7[0], number7[1], number7[2]);
  //send_char(0x06,number9[0], number9[1], number9[2]);
  //send_char(0x03,number8[0], number8[1], number8[2]);
  // delay(1000);
  // clear_VFD();  
}
void loop() {
  // clear_VFD();
  // msgHiFolks();
  // delay(1500);
  // //
  // all_ON();
  // delay(800);
  // //
  // led_on();
  // delay(100);
  // led_off();
  // delay(100);
  // //
  // characters();
  // numbers();
}
void vfdprint(String text){
  send_char(0x03,convert(text[0],0),convert(text[0],1),convert(text[0],2));
  send_char(0x06,convert(text[1],0),convert(text[1],1),convert(text[1],2));
  send_char(0x09,convert(text[2],0),convert(text[2],1),convert(text[2],2));
  send_char(0x0C,convert(text[3],0),convert(text[3],1),convert(text[3],2));
  send_char(0x0F,convert(text[4],0),convert(text[4],1),convert(text[4],2));
  send_char(0x12,convert(text[5],0),convert(text[5],1),convert(text[5],2));
  send_char(0x15,convert(text[6],0),convert(text[6],1),convert(text[6],2));
  send_char(0x18,convert(text[7],0),convert(text[7],1),convert(text[7],2));

}
