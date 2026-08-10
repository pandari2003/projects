#include <Wire.h>
#include <WiFiS3.h>
#include <Adafruit_PWMServoDriver.h>
 
// WIFI 

char ssid[] = "Telezer_J";
char pass[] = "Telezer12";

WiFiServer server(80);

 
// PCA9685 

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

 
// SERVO SETTINGS 

#define SERVO_MIN  110
#define SERVO_MAX  510


// Servo order:
//
// 0 = FL Hip
// 1 = FL Knee
// 2 = FR Hip
// 3 = FR Knee
// 4 = RL Hip
// 5 = RL Knee
// 6 = RR Hip
// 7 = RR Knee

 
// YOUR STAND POSE 

const int standPose[8] = {
  50, 50,
  120, 110,
  90, 60,
  80, 98
};

 
// SITTING POSE 
//
// STARTING VALUES.
// These may need adjustment. 

const int sitPose[8] = {
  50, 110,
  120, 50,
  90, 120,
  80, 40
};

 
// CURRENT POSE 

int currentPose[8] = {
  50, 50,
  120, 110,
  90, 60,
  80, 98
};

 
// ROBOT STATE 

enum RobotState {
  STAND,
  SIT,
  WALK,
  STOPPED
};

RobotState robotState = STAND;

 
// ANGLE TO PCA9685 PULSE 

int angleToPulse(int angle)
{
  angle = constrain(angle, 0, 180);

  return map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
}

 
// MOVE ONE SERVO 

void servoWrite(int servo, int angle)
{
  angle = constrain(angle, 0, 180);

  pwm.setPWM(
    servo,
    0,
    angleToPulse(angle)
  );
}

 
// MOVE TO POSE 

void moveToPose(const int targetPose[8])
{
  bool moving = true;

  while (moving)
  {
    moving = false;

    for (int i = 0; i < 8; i++)
    {
      if (currentPose[i] < targetPose[i])
      {
        currentPose[i]++;
        moving = true;
      }

      else if (currentPose[i] > targetPose[i])
      {
        currentPose[i]--;
        moving = true;
      }

      servoWrite(i, currentPose[i]);
    }

    delay(12);

    // Allow STOP button to be processed
    WiFiClient client = server.available();

    if (client)
    {
      String request = "";

      unsigned long timeout = millis();

      while (client.connected() &&
             millis() - timeout < 1000)
      {
        if (client.available())
        {
          char c = client.read();
          request += c;

          if (c == '\n')
            break;
        }
      }

      if (request.indexOf("GET /stop") >= 0)
      {
        robotState = STOPPED;
      }

      client.stop();
    }

    if (robotState == STOPPED)
      break;
  }
}

 
// STAND 

void standRobot()
{
  Serial.println("STAND");

  robotState = STAND;

  moveToPose(standPose);
}

 
// SIT 

void sitRobot()
{
  Serial.println("SIT");

  robotState = SIT;

  moveToPose(sitPose);
}

 
// WALK PHASE 1 

void walkPhase1()
{
  // FL leg
  servoWrite(0, 65);
  servoWrite(1, 30);

  // RR leg
  servoWrite(6, 65);
  servoWrite(7, 70);

  delay(250);

  servoWrite(1, 50);
  servoWrite(7, 98);

  delay(100);

  servoWrite(0, 35);
  servoWrite(6, 95);

  delay(250);
}

 
// WALK PHASE 2 

void walkPhase2()
{
  // FR leg
  servoWrite(2, 105);
  servoWrite(3, 80);

  // RL leg
  servoWrite(4, 110);
  servoWrite(5, 35);

  delay(250);

  servoWrite(3, 110);
  servoWrite(5, 60);

  delay(100);

  servoWrite(2, 135);
  servoWrite(4, 70);

  delay(250);
}

 
// WALK 

void walkRobot()
{
  Serial.println("WALK");

  // Start from stand
  moveToPose(standPose);

  robotState = WALK;

  while (robotState == WALK)
  {
    walkPhase1();

    handleWebClient();

    if (robotState != WALK)
      break;

    walkPhase2();

    handleWebClient();
  }
}

 
// WEB PAGE 

const char webpage[] = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta name="viewport"
content="width=device-width, initial-scale=1">

<title>Dog Robot</title>

<style>

body
{
  font-family: Arial;
  text-align: center;

  background: #222;
  color: white;

  margin-top: 40px;
}

h1
{
  font-size: 32px;
}

button
{
  width: 220px;
  height: 70px;

  margin: 10px;

  font-size: 24px;
  font-weight: bold;

  border: none;
  border-radius: 15px;
}

.stand
{
  background: green;
  color: white;
}

.sit
{
  background: blue;
  color: white;
}

.walk
{
  background: orange;
  color: black;
}

.stop
{
  background: red;
  color: white;
}

button:active
{
  transform: scale(0.95);
}

#status
{
  margin-top: 30px;
  font-size: 22px;
}

</style>

</head>


<body>

<h1>🐕 DOG ROBOT</h1>


<button
class="stand"
onclick="command('/stand')">

STAND

</button>


<br>


<button
class="sit"
onclick="command('/sit')">

SIT

</button>


<br>


<button
class="walk"
onclick="command('/walk')">

WALK

</button>


<br>


<button
class="stop"
onclick="command('/stop')">

STOP

</button>


<div id="status">

Status: Ready

</div>


<script>

function command(url)
{
  fetch(url)
  .then(response => response.text())

  .then(data =>
  {
    document.getElementById("status")
    .innerHTML = "Status: " + data;
  });
}

</script>


</body>

</html>

)rawliteral";

 
// WEB SERVER 

void handleWebClient()
{
  WiFiClient client = server.available();

  if (!client)
    return;


  String request = "";

  unsigned long timeout = millis();


  while (client.connected() &&
         millis() - timeout < 1000)
  {
    if (client.available())
    {
      char c = client.read();

      request += c;

      if (c == '\n')
        break;
    }
  }


  // ===================================================
  // STAND
  // ===================================================

  if (request.indexOf("GET /stand") >= 0)
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    client.println("STANDING");

    client.stop();

    standRobot();

    return;
  }


  // ===================================================
  // SIT
  // ===================================================

  if (request.indexOf("GET /sit") >= 0)
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    client.println("SITTING");

    client.stop();

    sitRobot();

    return;
  }


  // ===================================================
  // WALK
  // ===================================================

  if (request.indexOf("GET /walk") >= 0)
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    client.println("WALKING");

    client.stop();

    robotState = WALK;

    return;
  }


  // ===================================================
  // STOP
  // ===================================================

  if (request.indexOf("GET /stop") >= 0)
  {
    robotState = STOPPED;

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    client.println("STOPPED");

    client.stop();

    return;
  }


  // ===================================================
  // WEB PAGE
  // ===================================================

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.print(webpage);

  delay(1);

  client.stop();
}

 
// WIFI 

void connectWiFi()
{
  Serial.print("Connecting to WiFi");

  WiFi.disconnect();
  delay(1000);

  int status = WL_IDLE_STATUS;

  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(ssid, pass);

    Serial.print(".");
    
    delay(5000);
  }

  Serial.println();
  Serial.println("WiFi connected!");

  // Wait until DHCP gives us an IP address
  unsigned long startTime = millis();

  while (WiFi.localIP() == IPAddress(0, 0, 0, 0))
  {
    delay(500);

    Serial.print("#");

    if (millis() - startTime > 15000)
    {
      Serial.println();
      Serial.println("DHCP timeout!");
      break;
    }
  }

  Serial.println();

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Signal strength: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  server.begin();

  Serial.println("Web server started!");
}

 
// SETUP 

void setup()
{
  Serial.begin(115200);

  delay(1000);


  // PCA9685

  Wire.begin();

  pwm.begin();

  pwm.setOscillatorFrequency(27000000);

  pwm.setPWMFreq(50);

  delay(500);


  // Stand position

  for (int i = 0; i < 8; i++)
  {
    servoWrite(
      i,
      standPose[i]
    );
  }


  delay(1000);


  // WiFi

  connectWiFi();
}

 
// LOOP 

void loop()
{
  handleWebClient();

  if (robotState == WALK)
  {
    walkRobot();
  }
}