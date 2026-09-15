/*
 * ---------------------------------------------------------------------------
 * ESP32 Multi-Purpose Cleaning Robot
 * ---------------------------------------------------------------------------
 *
 * Features:
 *  - Bluetooth-based Manual Mode
 *  - Automatic path execution
 *  - L298N-based DC motor control
 *  - MPU6050-based turn-angle feedback
 *  - Cleaner, blower and water-pump control through relays
 *
 * Auto Paths:
 *  - Square
 *  - Rectangle
 *  - U1
 *  - U2
 *
 * Controller: ESP32
 * ---------------------------------------------------------------------------
 */

#include <Wire.h>
#include <MPU6050_light.h>
#include "BluetoothSerial.h"


// ---------------------------------------------------------------------------
// Bluetooth and MPU6050
// ---------------------------------------------------------------------------

BluetoothSerial SerialBT;
MPU6050 mpu(Wire);


// ---------------------------------------------------------------------------
// L298N Motor Driver Pins
// ---------------------------------------------------------------------------

#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 13

#define ENA 12
#define ENB 5


// ---------------------------------------------------------------------------
// Cleaning Mechanism Relay Pins
// ---------------------------------------------------------------------------

#define RELAY_CLEANER     25
#define RELAY_BLOWER      33
#define RELAY_WATER_PUMP  15


// ---------------------------------------------------------------------------
// PWM Configuration
// ---------------------------------------------------------------------------

const int PWM_FREQUENCY = 30000;
const int PWM_CHANNEL_LEFT = 0;
const int PWM_CHANNEL_RIGHT = 1;
const int PWM_RESOLUTION = 8;


// ---------------------------------------------------------------------------
// Robot Configuration
// ---------------------------------------------------------------------------

int motorSpeed = 200;

/*
 * Distance is estimated using movement time.
 * This value represents milliseconds of movement per centimetre.
 */
float timePerCm = 20.0;

/*
 * Default turn angle.
 * Slight calibration offset is used instead of exactly 90 degrees.
 */
float turnAngle = 90.2;


// ---------------------------------------------------------------------------
// Robot State
// ---------------------------------------------------------------------------

bool manualMode = true;
bool wasConnected = false;


// ---------------------------------------------------------------------------
// User-Configurable Auto Path Parameters
// ---------------------------------------------------------------------------

int userLength = 100;
int userBreadth = 100;
int userSpeed = 200;


// ---------------------------------------------------------------------------
// Function Prototypes
// ---------------------------------------------------------------------------

void stopMotors();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();

void moveForwardCm(int cm);
void turn90(char direction);

void runSquarePattern();
void rectangularPattern(int length, int breadth);
void customU1Pattern();
void customU2Pattern();

void manualControl(String command);
void setMotorSpeed(int newSpeed);
void checkRelayCommands();


// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);

    // Start Bluetooth
    SerialBT.begin("ESP32_MOTOR_BT");

    // Configure motor direction pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Configure cleaning relays
    pinMode(RELAY_CLEANER, OUTPUT);
    pinMode(RELAY_BLOWER, OUTPUT);
    pinMode(RELAY_WATER_PUMP, OUTPUT);

    // Make sure cleaning mechanisms are OFF during startup
    digitalWrite(RELAY_CLEANER, LOW);
    digitalWrite(RELAY_BLOWER, LOW);
    digitalWrite(RELAY_WATER_PUMP, LOW);

    // Configure PWM channels
    ledcAttachChannel(
        ENA,
        PWM_FREQUENCY,
        PWM_RESOLUTION,
        PWM_CHANNEL_LEFT
    );

    ledcAttachChannel(
        ENB,
        PWM_FREQUENCY,
        PWM_RESOLUTION,
        PWM_CHANNEL_RIGHT
    );

    // Initialize MPU6050
    Wire.begin();

    mpu.begin();

    delay(1000);

    // Gyroscope calibration
    mpu.calcGyroOffsets();

    // Ensure robot is stationary after initialization
    stopMotors();

    delay(1000);

    SerialBT.println("Select Mode:");
    SerialBT.println("1 = Manual Mode");
    SerialBT.println("2 = Auto Path Mode");
}


// ---------------------------------------------------------------------------
// Main Loop
// ---------------------------------------------------------------------------

void loop()
{
    // Continuously update MPU6050 measurements
    mpu.update();


    // -----------------------------------------------------------------------
    // Bluetooth Connection Handling
    // -----------------------------------------------------------------------

    if (SerialBT.hasClient() && !wasConnected)
    {
        wasConnected = true;

        SerialBT.println("Select Mode:");
        SerialBT.println("1 = Manual Mode");
        SerialBT.println("2 = Auto Path Mode");
    }

    if (!SerialBT.hasClient() && wasConnected)
    {
        wasConnected = false;
    }


    // -----------------------------------------------------------------------
    // Bluetooth Command Processing
    // -----------------------------------------------------------------------

    if (SerialBT.available())
    {
        String input = SerialBT.readStringUntil('\n');

        input.trim();


        // -------------------------------------------------------------------
        // Manual Mode
        // -------------------------------------------------------------------

        if (input == "1")
        {
            manualMode = true;

            SerialBT.println("Manual Mode Selected");
        }


        // -------------------------------------------------------------------
        // Auto Path Mode
        // -------------------------------------------------------------------

        else if (input == "2")
        {
            manualMode = false;

            SerialBT.println("Auto Path Mode Selected");
            SerialBT.println("Select Pattern:");

            SerialBT.println("z  = Square Pattern");
            SerialBT.println("p  = Rectangular Pattern");
            SerialBT.println("U1 = U1 Pattern");
            SerialBT.println("U2 = U2 Pattern");
        }


        // -------------------------------------------------------------------
        // Square Pattern
        // -------------------------------------------------------------------

        else if (!manualMode && input == "z")
        {
            SerialBT.println("Square Pattern Selected");

            SerialBT.println("Enter Side Length (cm):");

            while (!SerialBT.available())
            {
                checkRelayCommands();
            }

            userLength = SerialBT.parseInt();

            SerialBT.readStringUntil('\n');


            SerialBT.println("Enter Speed (0-255):");

            while (!SerialBT.available())
            {
                checkRelayCommands();
            }

            userSpeed = SerialBT.parseInt();

            SerialBT.readStringUntil('\n');


            SerialBT.println("Enter Turn Angle (degrees):");

            while (!SerialBT.available())
            {
                checkRelayCommands();
            }

            turnAngle = SerialBT.parseFloat();

            SerialBT.readStringUntil('\n');


            setMotorSpeed(userSpeed);

            runSquarePattern();
        }


        // -------------------------------------------------------------------
        // Rectangle Pattern
        // -------------------------------------------------------------------

        else if (!manualMode && input == "p")
        {
            SerialBT.println("Rectangular Pattern Selected");


            SerialBT.println("Enter Length (cm):");

            while (!SerialBT.available())
            {
                checkRelayCommands();
            }

            userLength = SerialBT.parseInt();

            SerialBT.readStringUntil('\n');


            SerialBT.println("Enter Breadth (cm):");

            while (!SerialBT.available())
            {
                checkRelayCommands();
            }

            userBreadth = SerialBT.parseInt();

            SerialBT.readStringUntil('\n');


            SerialBT.println("Enter Speed (0-255):");

            while (!SerialBT.available())
            {
                checkRelayCommands();
            }

            userSpeed = SerialBT.parseInt();

            SerialBT.readStringUntil('\n');


            setMotorSpeed(userSpeed);

            rectangularPattern(
                userLength,
                userBreadth
            );
        }


        // -------------------------------------------------------------------
        // U1 Pattern
        // -------------------------------------------------------------------

        else if (!manualMode && input == "U1")
        {
            SerialBT.println("U1 Pattern Selected");

            SerialBT.println("Enter Speed (0-255):");

            while (!SerialBT.available())
            {
                checkRelayCommands();
            }

            userSpeed = SerialBT.parseInt();

            SerialBT.readStringUntil('\n');


            setMotorSpeed(userSpeed);

            customU1Pattern();
        }


        // -------------------------------------------------------------------
        // U2 Pattern
        // -------------------------------------------------------------------

        else if (!manualMode && input == "U2")
        {
            SerialBT.println("U2 Pattern Selected");

            SerialBT.println("Enter Speed (0-255):");

            while (!SerialBT.available())
            {
                checkRelayCommands();
            }

            userSpeed = SerialBT.parseInt();

            SerialBT.readStringUntil('\n');


            setMotorSpeed(userSpeed);

            customU2Pattern();
        }


        // -------------------------------------------------------------------
        // Manual Motor / Cleaning Commands
        // -------------------------------------------------------------------

        else
        {
            manualControl(input);
        }
    }
}


// ---------------------------------------------------------------------------
// Stop Motors
// ---------------------------------------------------------------------------

void stopMotors()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    ledcWriteChannel(PWM_CHANNEL_LEFT, 0);
    ledcWriteChannel(PWM_CHANNEL_RIGHT, 0);
}


// ---------------------------------------------------------------------------
// Move Forward
// ---------------------------------------------------------------------------

void moveForward()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    ledcWriteChannel(PWM_CHANNEL_LEFT, motorSpeed);
    ledcWriteChannel(PWM_CHANNEL_RIGHT, motorSpeed);
}


// ---------------------------------------------------------------------------
// Move Backward
// ---------------------------------------------------------------------------

void moveBackward()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    ledcWriteChannel(PWM_CHANNEL_LEFT, motorSpeed);
    ledcWriteChannel(PWM_CHANNEL_RIGHT, motorSpeed);
}


// ---------------------------------------------------------------------------
// Turn Left
// ---------------------------------------------------------------------------

void turnLeft()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    ledcWriteChannel(PWM_CHANNEL_LEFT, motorSpeed);
    ledcWriteChannel(PWM_CHANNEL_RIGHT, motorSpeed);
}


// ---------------------------------------------------------------------------
// Turn Right
// ---------------------------------------------------------------------------

void turnRight()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    ledcWriteChannel(PWM_CHANNEL_LEFT, motorSpeed);
    ledcWriteChannel(PWM_CHANNEL_RIGHT, motorSpeed);
}


// ---------------------------------------------------------------------------
// Move Forward by Approximate Distance
// ---------------------------------------------------------------------------

void moveForwardCm(int distanceCm)
{
    const int STEP_CM = 10;

    for (int distance = 0;
         distance < distanceCm;
         distance += STEP_CM)
    {
        moveForward();

        delay(STEP_CM * timePerCm);

        stopMotors();

        /*
         * Allows relay and stop commands to be checked between
         * movement segments.
         */
        checkRelayCommands();
    }
}


// ---------------------------------------------------------------------------
// MPU6050-Based Turn
// ---------------------------------------------------------------------------

void turn90(char direction)
{
    float startAngle = mpu.getAngleZ();

    float targetAngle;

    if (direction == 'L')
    {
        targetAngle = startAngle - turnAngle;
    }
    else
    {
        targetAngle = startAngle + turnAngle;
    }


    // Normalize target to -180 ... +180 degrees
    if (targetAngle > 180)
    {
        targetAngle -= 360;
    }

    if (targetAngle < -180)
    {
        targetAngle += 360;
    }


    if (direction == 'L')
    {
        turnLeft();
    }
    else
    {
        turnRight();
    }


    while (true)
    {
        mpu.update();

        float currentAngle = mpu.getAngleZ();


        if (currentAngle > 180)
        {
            currentAngle -= 360;
        }

        if (currentAngle < -180)
        {
            currentAngle += 360;
        }


        if (
            (direction == 'L' && currentAngle <= targetAngle) ||
            (direction == 'R' && currentAngle >= targetAngle)
           )
        {
            break;
        }

        delay(5);
    }


    stopMotors();
}


// ---------------------------------------------------------------------------
// Square Pattern
// ---------------------------------------------------------------------------

void runSquarePattern()
{
    for (int side = 0; side < 4; side++)
    {
        moveForwardCm(userLength);

        delay(200);

        turn90('R');

        delay(200);
    }

    stopMotors();
}


// ---------------------------------------------------------------------------
// Rectangle Pattern
// ---------------------------------------------------------------------------

void rectangularPattern(int length, int breadth)
{
    for (int cycle = 0; cycle < 2; cycle++)
    {
        moveForwardCm(length);

        turn90('R');

        moveForwardCm(breadth);

        turn90('R');
    }

    stopMotors();
}


// ---------------------------------------------------------------------------
// U1 Pattern
// ---------------------------------------------------------------------------

void customU1Pattern()
{
    const int DISTANCE_CM = 100;

    moveForwardCm(DISTANCE_CM);

    delay(200);

    turn90('L');

    delay(200);

    turn90('L');

    delay(200);

    moveForwardCm(DISTANCE_CM);

    delay(200);

    moveForwardCm(DISTANCE_CM);

    delay(200);

    turn90('R');

    delay(200);

    turn90('R');

    delay(200);

    moveForwardCm(DISTANCE_CM);

    stopMotors();
}


// ---------------------------------------------------------------------------
// U2 Pattern
// ---------------------------------------------------------------------------

void customU2Pattern()
{
    const int DISTANCE_CM = 100;

    moveForwardCm(DISTANCE_CM);

    delay(200);

    turn90('R');

    delay(200);

    turn90('R');

    delay(200);

    moveForwardCm(DISTANCE_CM);

    delay(200);

    moveForwardCm(DISTANCE_CM);

    delay(200);

    turn90('L');

    delay(200);

    turn90('L');

    delay(200);

    moveForwardCm(DISTANCE_CM);

    stopMotors();
}


// ---------------------------------------------------------------------------
// Manual Commands
// ---------------------------------------------------------------------------

void manualControl(String command)
{
    if (command.length() == 0)
    {
        return;
    }


    char cmd = command.charAt(0);


    switch (cmd)
    {
        // Motor control
        case 'f':
            moveForward();
            break;

        case 'b':
            moveBackward();
            break;

        case 'l':
            turnLeft();
            break;

        case 'r':
            turnRight();
            break;

        case 's':
            stopMotors();
            break;


        // Cleaner
        case '5':
            digitalWrite(RELAY_CLEANER, HIGH);
            break;

        case '6':
            digitalWrite(RELAY_CLEANER, LOW);
            break;


        // Air blower
        case '3':
            digitalWrite(RELAY_BLOWER, HIGH);
            break;

        case '4':
            digitalWrite(RELAY_BLOWER, LOW);
            break;


        // Water pump
        case '7':
            digitalWrite(RELAY_WATER_PUMP, HIGH);
            break;

        case '8':
            digitalWrite(RELAY_WATER_PUMP, LOW);
            break;


        // Software stop command
        case 'T':

            stopMotors();

            digitalWrite(RELAY_CLEANER, LOW);
            digitalWrite(RELAY_BLOWER, LOW);
            digitalWrite(RELAY_WATER_PUMP, LOW);

            SerialBT.println(
                "STOP: Motors and cleaning mechanisms turned OFF."
            );

            break;
    }
}


// ---------------------------------------------------------------------------
// Motor Speed Configuration
// ---------------------------------------------------------------------------

void setMotorSpeed(int newSpeed)
{
    newSpeed = constrain(newSpeed, 0, 255);

    motorSpeed = newSpeed;

    ledcWriteChannel(
        PWM_CHANNEL_LEFT,
        motorSpeed
    );

    ledcWriteChannel(
        PWM_CHANNEL_RIGHT,
        motorSpeed
    );

    SerialBT.printf(
        "Speed set to %d\n",
        motorSpeed
    );
}


// ---------------------------------------------------------------------------
// Relay / Stop Command Check
// ---------------------------------------------------------------------------

void checkRelayCommands()
{
    if (SerialBT.available())
    {
        String relayCommand =
            SerialBT.readStringUntil('\n');

        relayCommand.trim();

        manualControl(relayCommand);
    }
}
