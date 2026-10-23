#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

//====================
// WiFi Credentials
//====================
const char* WIFI_SSID = "Telezer_J";
const char* WIFI_PASSWORD = "Telezer12";

//====================
// Servo Pins
//====================
#define LEFT_LEG_PIN     14
#define RIGHT_LEG_PIN    13
#define LEFT_FOOT_PIN    26
#define RIGHT_FOOT_PIN   27

//====================
// Home Position
//====================
int homeLeftLeg   = 90;
int homeRightLeg  = 90;
int homeLeftFoot  = 90;
int homeRightFoot = 90;

//====================
// Dance Angles
//====================
const int STEP_ANGLE = 25;
const int TILT_ANGLE = 20;

//====================
// Servo Objects
//====================
Servo legLeft;
Servo legRight;
Servo footLeft;
Servo footRight;

//====================
// Web Server
//====================
WebServer server(80);

bool busy = false;

enum RobotState
{
    STOPPED,
    FORWARD,
    BACKWARD,
    DANCE
};

RobotState robotState = STOPPED;

//==================================================
// Move Servo
//==================================================
void moveServo(Servo &servo, int angle, int waitTime = 200)
{
    angle = constrain(angle, 0, 180);
    servo.write(angle);
    delay(waitTime);
}

//==================================================
// Home Position
//==================================================
void home()
{
    moveServo(legLeft, homeLeftLeg);
    moveServo(legRight, homeRightLeg);
    moveServo(footLeft, homeLeftFoot);
    moveServo(footRight, homeRightFoot);
}
void stopRobot()
{
    home();
    busy = false;
}

//==================================================
// Dance Routine
//==================================================

void forward(int cycles = 2)
{
  for (int i = 0; i < cycles; i++)
  {
    moveServo(footLeft, homeLeftFoot + TILT_ANGLE);
    moveServo(footRight, homeRightFoot - TILT_ANGLE);

    moveServo(legLeft, homeLeftLeg - STEP_ANGLE);
    moveServo(legRight, homeRightLeg + STEP_ANGLE);

    moveServo(footLeft, homeLeftFoot - TILT_ANGLE);
    moveServo(footRight, homeRightFoot + TILT_ANGLE);

    moveServo(legLeft, homeLeftLeg + STEP_ANGLE);
    moveServo(legRight, homeRightLeg - STEP_ANGLE);
  }

  //home();
}

void backward(int cycles = 2)
{
  for (int i = 0; i < cycles; i++)
  {
    moveServo(footLeft, homeLeftFoot + TILT_ANGLE);
    moveServo(footRight, homeRightFoot - TILT_ANGLE);

    moveServo(legLeft, homeLeftLeg + STEP_ANGLE);
    moveServo(legRight, homeRightLeg - STEP_ANGLE);

    moveServo(footLeft, homeLeftFoot - TILT_ANGLE);
    moveServo(footRight, homeRightFoot + TILT_ANGLE);

    moveServo(legLeft, homeLeftLeg - STEP_ANGLE);
    moveServo(legRight, homeRightLeg + STEP_ANGLE);
  }

  //home();
}

void oneLegDance()
{
    // Lift LEFT leg
    moveServo(footLeft, homeLeftFoot + 20);
    moveServo(footRight, homeRightFoot - 20);

    moveServo(legLeft, homeLeftLeg - 30);

    for(int i=0;i<3;i++)
    {
        moveServo(legLeft, homeLeftLeg - 40,80);
        moveServo(legLeft, homeLeftLeg - 20,80);
    }

    home();

    // Lift RIGHT leg
    moveServo(footLeft, homeLeftFoot - 20);
    moveServo(footRight, homeRightFoot + 20);

    moveServo(legRight, homeRightLeg + 30);

    for(int i=0;i<3;i++)
    {
        moveServo(legRight, homeRightLeg + 40,80);
        moveServo(legRight, homeRightLeg + 20,80);
    }

    home();
}

//==================================================
// HTML Page
//==================================================
void handleRoot()
{
String page = R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<title>CUBY Robot Controller</title>

<style>

body{
    margin:0;
    background:#1a1a1a;
    font-family:Arial,Helvetica,sans-serif;
    color:white;
    text-align:center;
}

h1{
    margin-top:30px;
    font-size:40px;
    color:#00E5FF;
}

h2{
    color:#CCCCCC;
    margin-bottom:40px;
}

.container{
    display:flex;
    flex-direction:column;
    align-items:center;
}

button{
    width:240px;
    height:70px;
    margin:12px;
    border:none;
    border-radius:18px;
    font-size:26px;
    font-weight:bold;
    cursor:pointer;
    transition:.2s;
}

button:hover{
    transform:scale(1.05);
}

.forward{
    background:#00C853;
    
    color:white;
}

.backward{
    background:#D50000;
    color:white;
}

.stop{
    background:#FFC107;
    color:black;
}

.status{
    margin-top:30px;
    font-size:22px;
    color:#00E5FF;
}

</style>

</head>

<body>

<h1>CUBY ROBOT</h1>

<h2>WiFi Controller</h2>

<div class="container">

<button class="forward"
onclick="sendCommand('forward')">
FORWARD
</button>

<button class="backward"
onclick="sendCommand('backward')">
BACKWARD
</button>

<button class="forward"
onclick="sendCommand('dance')">
DANCE
</button>

<button class="stop"
onclick="sendCommand('stop')">
STOP
</button>

</div>

<div class="status" id="status">
Ready
</div>

<script>

function sendCommand(cmd)
{
    document.getElementById("status").innerHTML="Sending "+cmd+"...";

    fetch("/"+cmd)
    .then(response=>response.text())
    .then(data=>{
        document.getElementById("status").innerHTML=data;
    });
}

</script>

</body>

</html>

)rawliteral";

server.send(200,"text/html",page);

}

//==================================================
// Dance API
//==================================================


//==================================================
// Setup
//==================================================
void setup()
{
    Serial.begin(115200);

    delay(2000);
    Serial.println();
    Serial.println("ESP32 Started");

    // Allocate timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    // Servo Frequency
    legLeft.setPeriodHertz(50);
    legRight.setPeriodHertz(50);
    footLeft.setPeriodHertz(50);
    footRight.setPeriodHertz(50);

    // Attach Servos
    legLeft.attach(LEFT_LEG_PIN,500,2400);
    legRight.attach(RIGHT_LEG_PIN,500,2400);
    footLeft.attach(LEFT_FOOT_PIN,500,2400);
    footRight.attach(RIGHT_FOOT_PIN,500,2400);

    // Home Position
    home();

    // WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Connecting");
    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    // Web Pages
server.on("/", handleRoot);

server.on("/forward", []()
{
    robotState = FORWARD;
    server.send(200, "text/plain", "Forward");
});

server.on("/backward", []()
{
    robotState = BACKWARD;
    server.send(200, "text/plain", "Backward");
});

server.on("/dance", []()
{
    robotState = DANCE;
    server.send(200, "text/plain", "Dance");
});

server.on("/stop", []()
{
    robotState = STOPPED;
    home();
    server.send(200, "text/plain", "Stopped");
});

server.begin();

    Serial.println("Server Started");


}

//==================================================
// Loop
//==================================================
void loop()
{
    server.handleClient();

    if(robotState == FORWARD)
    {
        forward(1);
    }
    else if(robotState == BACKWARD)
    {
        backward(1);
    }
    else if(robotState == DANCE)
    {
        oneLegDance();
    }

    delay(10);
}