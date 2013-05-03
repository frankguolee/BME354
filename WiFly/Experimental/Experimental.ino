/*********************************************************

WiFi Electric Blanket Controler
Frank Lee
Adapted from Joel Bartlett
SparkFun Electronics
May 2nd, 2013

***********************************************************/

#include "WiFly.h"//include the WiFly experimental library
// #include "Credentials.h" 

// String Link = "https://dl.dropboxusercontent.com/u/8234850/Nelly%20-%20Hot%20In%20Herre%20short.mov";

char msg[128];//variables for reading in the html responses
int letterCount = 0; 

int ovenState;//variables for knowing the state the oven is in
int tempState;
int tempCount;

Server server(80);// I am on port 80

int powerSupply = 2;//variables for reading and contolling the buttons on the controller
int sensor = A5;
///////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(powerSupply, OUTPUT);
  pinMode(sensor, INPUT);
  digitalWrite(sensor, HIGH);

  WiFly.begin();

  Serial.begin(9600);
  Serial.print("IP: ");
  Serial.println(WiFly.ip());//taking this line out made the  wifly hang, so I just left it in
  
  server.begin();

  if(digitalRead(powerSupply) == LOW)// read the state of the blanket (on or off) upon startup to set the variable accordingly 
      ovenState = 0;
  if(digitalRead(powerSupply) == HIGH)
      ovenState = 1;
  //Upon hooking up the blanket to the Arduino, it needs to be set at 5 so it and the counter are synced up    
  tempState = 1;//Medium
  tempCount = 5;//Medium
  // Frank Lee change this
}
///////////////////////////////////////////////////////////////////
void loop() 
{

  Client client = server.available();
  //the flag dataFlag give a notification of where the actual post
  //is once we flag it we can begin recording the message
  //so we can do stuff with it later on
  
  int dataFlag = 0;
  int msgIsolator = 0;

  //int inChar; //variable for reading in data from the phone/browser 
  //boolean isParsing = false;// flag for reading in and parsing data
  
  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    boolean endOfCode = true;
    char c;
    
    while (client.connected()) 
    {
      if (client.available()) 
      {
        c = client.read();
        delay(10);
        //Uncomment this line to see the HTTP respone
        //Serial.print(c);
        
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (!client.available()) {
          endOfCode = true;
        } else if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
          endOfCode = false;
        }

        //this is where the message that the user entered is going so this is where
        //we record it into a char array
        if (msgIsolator == 1) 
        {
          recordMessage(c);
          delay(100);
        }

        if ((c == '\n' && current_line_is_blank && !client.available()) || endOfCode) 
        {
          
          //As soon as the page has been refreshed the letter count is set to zero
          letterCount = 0;
          checkAction();
          tempCount = digitalRead(sensor);
          tempCount = tempCount + 26;
          //Once the page has been refreshed we are no longer on the first run through
          //off the program so we set this to false
          endOfCode = false;
          
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
                        
          //This basically creates what the user will see on the site. 
          client.println("<html>");
          client.println("<title>Oven Control</title>");
          client.println("<h1><center>");
          
          // Nelly: Hot in Here
          client.println("<img src=\"http://livelyproductions.org/home/wp-content/uploads/2013/04/proper-undress.jpg\">");
          client.println("<br>");
          client.println("<embed height=\"50\" width=\"400\" src=\"https://dl.dropboxusercontent.com/u/8234850/Hot%20In%20Herre.mp3\">");
          // Elvis Presley: Burning Love
//          client.println("<img src=\"http://2.bp.blogspot.com/-SIu3abnSiak/T-8t2LoPAqI/AAAAAAAAArM/D51y1MlFSgk/s1600/Elvis-Presley-8.jpg\">");
//          client.println("<br>");
//          client.println("<embed height=\"50\" width=\"500\" src=\"https://dl.dropboxusercontent.com/u/8234850/Burning%20Love.mp3\">");
          client.println("</center></h1>");
          
          
          
          //the form code creates the buttons to click and change the temp/ovenState 
          client.println("<center>");
          client.println("Turn Oven On");
          client.println("<form name=\"input\" action=\"\" method=\"post\">");
          client.println("<input type=\"submit\" name=\"%\" value=\"On\" style=\"height:50px; width:150px\" >");// I made the buttons larger for smart phone screens 
          client.println("</form>");
          
          client.println("Turn Oven Off");
          client.println("<form name=\"input\" action=\"\" method=\"post\">");
          client.println("<input type=\"submit\" name=\"%\" value=\"Off\" style=\"height:50px; width:150px\" >");
          client.println("</form>");
          
//          client.println("<h1>Camera control</h1>");
//          client.println("<form method=get name=SERVO>");
//          client.println("<input type=submit value=UP name=UP style=\"width:100px\"><br>");
//          client.println("<input type=submit value=LT name=LT style=\"width:100px\"><input type=submit value=CN name=CN style=\"width:100px\"><input type=submit value=RT name=RT style=\"width:100px\"><br>");
//          client.println("<input type=submit value=DN name=DN style=\"width:100px\">");
//          client.println("<hr />");
//          client.print("<iframe width= 640 height= 360  src=");
//          client.print(Link);
//          client.println(" frameborder= 0  allowfullscreen></iframe>");
//          client.println("</form>");

//          client.println("<embed src=\"https://dl.dropboxusercontent.com/u/8234850/Nelly%20-%20Hot%20In%20Herre%20short.mov\">");

          
//          client.println("Set Oven Temperature");
//          client.println("<form name=\"input\" action=\"\" method=\"post\">");
//          client.println("<input type=\"text\" >");
//          client.println("</form>");
//          Frank Lee
//          
//          client.println("<form name=\"input\" action=\"\" method=\"post\">");
//          client.println("<input type=\"submit\" name=\"%\" value=\"High\" style=\"height:50px; width:150px\" >");
//          client.println("</form>");
//          
//          client.println("<form name=\"input\" action=\"\" method=\"post\">");
//          client.println("<input type=\"submit\" name=\"%\" value=\"Medium\" style=\"height:50px; width:150px\" >");
//          client.println("</form>");
//          
//          client.println("<form name=\"input\" action=\"\" method=\"post\">");
//          client.println("<input type=\"submit\" name=\"%\" size=\"8\" value=\"Low\" style=\"height:50px; width:150px\" >");
//          client.println("</form>");
          
          //This prints out the state of each variable on the webpage
          client.println("<h4>");
          client.print("Oven is currently: ");
          if(ovenState == 0)
            client.println("<span style=\"color:green;\">OFF</span>");
          else
            client.println("<span style=\"color:red;\">ON</span>");
            client.println("</h4>");
          
          client.println("<h4>");
          client.print("Current Temperature: ");
          client.println(tempCount);
          if(tempState == 0)
            client.println("<span style=\"color:yellow;\">LOW</span>");//span code changes the color of the text
          else if(tempState == 1)
            client.println("<span style=\"color:orange;\">degrees Celsius</span>");
          else
            client.println("<span style=\"color:red;\">HIGH</span>");
            client.println("</h4>");
          
          client.println("<form name=\"input\" action=\"\" method=\"post\">");
          client.println("<input type=\"submit\" name=\"%\" size=\"8\" value=\"Reset\" style=\"height:50px; width:150px\" >");
          client.println("</form>");
          // client.println("<form name=\"input\" action=\"\" method=\"post\">");
          // client.println("<input type=\"submit\" name=\"%\" size=\"8\" value=\"Reload\" style=\"height:50px; width:150px\" >");
          // client.println("</form>");
      
          client.println("</center>");
          client.println("</html>");
          //***NOTE: I originally added the Reload button, and it broke everything. I combined the temperature state and number into one line and 
          //took out some other html code. It then worked. This leads me to beleive that there is only so much that the WiFly can serve up at a time??*** 
          delay(500);//delay is very important 
          letterCount = 0;
          break;
        }
        
        //if you want to see all data comming in and going out uncomment the line below
        Serial.print(c);
       
        //We detect where the actual post data is in other words what the user entered
        //once we isolate it we can parse it and use it
        if (c =='%')
        {
          dataFlag = 1;
        }
          if (dataFlag == 1)
          {
            //in here goes the user entered data it follows the string "25="
            if (c == '=')
            {
              //trim the fat of the soon to be recorded char so we only get the user entered message
              msgIsolator = 1;
            }
          }
      }
    }
    
  // give the web browser time to receive the data
  delay(100);//delay is very important 
  client.flush();
  client.stop();
  }//if(client)

  //If the buttons ont he controller are pressed manually, keep track of each press. 
  // if(digitalRead(upBut) == LOW && digitalRead(powerSupply) == HIGH)//if the up button is pressed and the unit is on...
  // {
  //   tempCount++;
  //   delay(200);//debounce
  //   if(tempCount > 10)//if temp count is already HIGH, stay at 10
  //   tempCount = 10; 
  // }
  // if(digitalRead(downBut) == LOW && digitalRead(powerSupply) == HIGH)//if the down button is pressed and the unit is on...
  // {
  //   tempCount--;
  //   delay(200);//debounce
  //   if(tempCount < 1)//if temp count is already LOW, stay at 1
  //   tempCount = 1; 
  // }
  
  if(digitalRead(powerSupply) == LOW)//If no power is read on the unit, it is off.
      ovenState = 0;
  if(digitalRead(powerSupply) == HIGH)//If power is read on the unit, it is on.
      ovenState = 1;
  
}//end loop
///////////////////////////////////////////////////////////////////////

char recordMessage (char incomingMsg)
{
  //letterCount = 0;
  msg[letterCount] = incomingMsg;
  letterCount++;
  delay(100);
}
///////////////////////////////////////////////////////////////////////
 void checkAction() 
 // the first two or three letters of each message are read to determine which button was clicked on the webage
 { 
//   if (msg[0] == 'H' && msg[1] == 'i' && msg[2] == 'g')// set to HIGH
//   {
//     if(tempCount < 10)//if in MED or LOW, go up to HIGH
//     {
//     pinMode(upBut, OUTPUT);//set button to output to control controller
//       do
//       {
//         digitalWrite(upBut, LOW);//simulate button presses
//         delay(100);
//         digitalWrite(upBut, HIGH);
//         delay(100);
//         tempCount++;
//       }while(tempCount < 10);
//     pinMode(upBut, INPUT);//set back to INPUT to allow manual presses
//     }
    
//     tempState = 2;
//     //The Err is set at the end of each command to prevent the regualr commands from being sent again on a browser reload 
//     msg[0] = 'E';
//     msg[1] = 'r';
//     msg[2] = 'r';
//   }
//   //-------------------------------------------------------
//   else if (msg[0] == 'M' && msg[1] == 'e' && msg[2] == 'd')//Set to Medium
//   {
//     if(tempCount < 5)//if in LOW, go up to MED
//     {
//     pinMode(upBut, OUTPUT);
//       do
//       {
//         digitalWrite(upBut, LOW);
//         delay(100);
//         digitalWrite(upBut, HIGH);
//         delay(100);
//         tempCount++;
//       }while(tempCount < 5);
//     pinMode(upBut, INPUT);
//     }
//     if(tempCount > 5)//if in HIGH, go down to MED
//     {
//     pinMode(downBut, OUTPUT);
//       do
//       {
//         digitalWrite(downBut, LOW);
//         delay(100);
//         digitalWrite(downBut, HIGH);
//         delay(100);
//         tempCount--;
//       }while(tempCount > 5);
//     pinMode(downBut, INPUT);
//     }
    
//     tempState = 1;
//     msg[0] = 'E';
//     msg[1] = 'r';
//     msg[2] = 'r';
//   }
//   //-------------------------------------------------------
//   else if (msg[0] == 'L' && msg[1] == 'o' && msg[2] == 'w')//Set to LOW
//   {
//     if(tempCount > 1)//if in HIGH or MED, go down to LOW
//     {
//     pinMode(downBut, OUTPUT);
//       do
//       {
//         digitalWrite(downBut, LOW);
//         delay(100);
//         digitalWrite(downBut, HIGH);
//         delay(100);
//         tempCount--;
//       }while(tempCount > 1);
//     pinMode(downBut, INPUT);
//     }

//     tempState = 0;
//     msg[0] = 'E';
//     msg[1] = 'r';
//     msg[2] = 'r';
//   }
   //-------------------------------------------------------
   if (msg[0] == 'O' && msg[1] == 'f' && msg[2] == 'f')//Set blanket to OFF
   { 
     if(ovenState == 1)//only turn blanket OFF if it's already ON
     {
     pinMode(sensor, OUTPUT);
     digitalWrite(sensor, LOW);
     delay(100);
     digitalWrite(sensor, HIGH);
     pinMode(sensor, INPUT);
     }

     ovenState = 0;
     digitalWrite(powerSupply, LOW);
     msg[0] = 'E';
     msg[1] = 'r';
     msg[2] = 'r';
   }
   //-------------------------------------------------------
   else if (msg[0] == 'O' && msg[1] == 'n')//Set blanket to ON
   {
     if(ovenState == 0)//only turn blanket ON if it's already OFF
     {
     pinMode(sensor, OUTPUT);
     digitalWrite(sensor, LOW);
     delay(100);
     digitalWrite(sensor, HIGH);
     pinMode(sensor, INPUT);
     }

     ovenState = 1;
     digitalWrite(powerSupply, HIGH);
     msg[0] = 'E';
     msg[1] = 'r';
     msg[2] = 'r';
}
//   //-------------------------------------------------------
//     else if (msg[0] == 'R' && msg[1] == 'e' && msg[2] == 'l')//Reload browser
//   {
//     //do nothing but refresh 
//     msg[0] = 'E';
//     msg[1] = 'r';
//     msg[2] = 'r';
//   }
//   //-------------------------------------------------------
   else if (msg[0] == 'R' && msg[1] == 'e' && msg[2] == 's')// Reset variables if blanket and Arduino become out of Sync
   {
       tempState = 1;//Med
       tempCount = 5;//Med
      
       msg[0] = 'E';
       msg[1] = 'r';
       msg[2] = 'r';
   }
  
 
}//end checkAction()

