//模組上所有腳位都要接上, 否則實驗可能失敗
//VIN(VBATT)１２Ｖ，如果沒接ELM327晶片沒電源
//
//通訊埠
//G 接 Arduino GND
//T 接 Arduino 0(RX)
//R 接 Arduino 1(TX)
//
//
// include the library code:

#include <SoftwareSerial.h>

SoftwareSerial mybt(8, 9);
uint16_t value;
byte pid; 
int timeo = 100;
int t_delay = 70;
byte ble_header = 0xEA;
byte ble_tail = 0xBB;
int CmdCount=1;
byte inData;
char inChar;
String BuildINString="";
String DisplayString="";
String work_pid="";
long DisplayValue;
String SentMessage="";
int ByteCount=0;
long A;
int B;
int WorkingVal;
String WorkingString="";
int RefreshCounter=0; // when the reading should refresh
int RefreshCounterMax=50; //Adjust this refreshrate
int RESETMenuName=0;

//MenuAnalogVal
int x;
//Declare Buttons
int MenuID=0;
int UpDownMenu=0;


void setup() 
{
  
  Serial.begin(38400);
  Serial1.begin(38400);
  mybt.begin(115200);
  delay(1000);
  //Send a test message to see if the ELM327 Chip is responding
  SentMessage = "ATI";
  Serial.println("ATZ");
  Serial1.println("ATZ");delay(1500);ReadData();
  Serial.println("ATI");
  Serial1.println("ATI");delay(500);ReadData();
 
    
  Retry_connect:
  Serial.println("Connecting......    ");
  Serial1.println("ATE0");delay(500);ReadData();
  Serial1.println("ATL0");delay(500);ReadData();
  Serial1.println("ATSP0");delay(500);ReadData();
  Serial1.println("0100");delay(5000);ReadData();
  if (BuildINString.substring(4,9)=="41 00")
    {
      Serial.println("Connect......    ");
    }
    else
    {

      delay(1500);
      goto Retry_connect;
    }
  
  delay(1500);
}

void loop() 
{



//  Serial.println("01 Coolant Temp    ");SentMessage = "01 05";mySerial.println("01 05");delay(t_delay);ReadData();delay(timeo);
//  Serial.println("02 IAT Temp        ");SentMessage = "01 0F";mySerial.println("01 0F");delay(t_delay);ReadData();delay(timeo);
//  Serial.println("03 Ambient Temp    ");SentMessage = "01 46";mySerial.println("01 46");delay(t_delay);ReadData();delay(timeo);
  // 油門開度
      Serial.println("04 Throttle %   ");
      SentMessage = "01 11";Serial1.flush();Serial1.println("01 11");delay(t_delay);ReadData();delay(timeo);
  
  // 轉速
      Serial.println("09 RPM             ");
      SentMessage = "01 0C";Serial1.flush();Serial1.println("01 0C");delay(t_delay);ReadData();delay(timeo);

  
  // 車速

      Serial.println("10 Vehicle Speed   ");
      SentMessage = "01 0D";Serial1.flush();Serial1.println("01 0D");delay(t_delay);ReadData();delay(timeo);

      Serial.println("11 Air Flow Rate  ");
      SentMessage = "01 10";Serial1.flush();Serial1.println("01 10");delay(t_delay);ReadData();delay(timeo);

      Serial.println("12 Barometric     ");
      SentMessage = "01 33";Serial1.flush();Serial1.println("01 33");delay(t_delay);ReadData();delay(timeo);



}
void bt_write(String pid, String value){
  String writevalue = "AA,"+pid+","+value+",";
  mybt.println(writevalue);
}
//Read Data and act accordingly
void ReadData()
{

  BuildINString="";  
  while(Serial1.available())
  {
    inData=0;
    inChar=0;
    inData = Serial1.read();
    inChar=char(inData);
    BuildINString = BuildINString + inChar;
  }
  Serial1.flush();
  
  //
  //if(BuildINString!=""){mySerial.print(BuildINString);}
  Serial.print("Recv: ");

      Serial.println(BuildINString);
      BuildINString.replace(SentMessage,"");
  BuildINString.replace(">","");
  BuildINString.replace("OK","");
  BuildINString.replace("STOPPED","");
  BuildINString.replace("SEARCHING","");
  BuildINString.replace("NO DATA","");
  BuildINString.replace("?","");
  BuildINString.replace(",","");
  //mySerial.print(BuildINString);
  
  //Check which OBD Command was sent and calculate VALUE 
  //Calculate RPM I.E Returned bytes wil be 41 0C 1B E0 
  //RPM
  if (BuildINString.indexOf("41 0C")>-1)
  {
    WorkingString = BuildINString.substring(6,8);
    A = strtoul(WorkingString.c_str(),NULL,16);
    WorkingString = BuildINString.substring(9,11);
    B = strtoul(WorkingString.c_str(),NULL,16);
   DisplayValue = ((A * 256)+B)/4;
      DisplayString = String(DisplayValue);
   bt_write("0C",DisplayString);
      Serial.println(DisplayString);
  }
  //Calculate Vehicle speed I.E Returned bytes wil be 41 0C 1B E0 
  if (BuildINString.indexOf("41 0D")>-1)
  {
    WorkingString = BuildINString.substring(6,8);
    A = strtoul(WorkingString.c_str(),NULL,16);
    DisplayValue = A;
    DisplayString = String(DisplayValue);
    bt_write("0D",DisplayString);
       DisplayString = String(DisplayValue) + " km/h              ";
       Serial.println(DisplayString); 
  }
  
  
  //Air flow Rate
  if (BuildINString.indexOf("41 10")>-1)
  {
    WorkingString = BuildINString.substring(6,8);
    A = strtoul(WorkingString.c_str(),NULL,16);
    WorkingString = BuildINString.substring(10,12);
    B = strtoul(WorkingString.c_str(),NULL,16);
    DisplayValue = ((A * 256)+B)/100;
    DisplayString = String(DisplayValue);
   bt_write("10",DisplayString);
       DisplayString = String(DisplayValue) + " g/s            ";
       Serial.println(DisplayString); 
  }

  
  //Throttle position 
  if (BuildINString.indexOf("7F 01")>-1)
  {
    WorkingString = BuildINString.substring(6,8);
    A = strtoul(WorkingString.c_str(),NULL,16);
    DisplayValue = A *100/255;
    DisplayString = String(DisplayValue);
     bt_write("11",DisplayString);
   DisplayString = String(DisplayValue) + " %              ";
   Serial.println(DisplayString); 
  }
  //Barometric pressure
  if (BuildINString.indexOf("41 33")>-1)
  {
    WorkingString = BuildINString.substring(6,8);
    A = strtoul(WorkingString.c_str(),NULL,16);
    DisplayValue = A;
    DisplayString = String(DisplayValue);
     bt_write("33",DisplayString);
       DisplayString = String(DisplayValue) + " kpa              ";
       Serial.println(DisplayString); 
  }

      
  

 

}



