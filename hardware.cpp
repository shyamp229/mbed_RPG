// This header has all the (extern) declarations of the globals.
// "extern" means "this is instantiated somewhere, but here's what the name
// means.
#include "globals.h"

#include "hardware.h"

// We need to actually instantiate all of the globals (i.e. declare them once
// without the extern keyword). That's what this file does!

// Hardware initialization: Instantiate all the things!
uLCD_4DGL uLCD(p9,p10,p11);             // LCD Screen (tx, rx, reset)
//SDFileSystem sd(p5, p6, p7, p8, "sd");  // SD Card(mosi, miso, sck, cs)
Serial pc(USBTX,USBRX);                 // USB Console (tx, rx)
MMA8452 acc(p28, p27, 100000);        // Accelerometer (sda, sdc, rate)
DigitalIn button1(p21);                 // Pushbuttons (pin)
DigitalIn button2(p22);
DigitalIn button3(p23);
AnalogOut DACout(p18);                  // Speaker (pin)
PwmOut  speaker(p26);
wave_player waver(&DACout);
GameInputs in;


// Some hardware also needs to have functions called before it will set up
// properly. Do that here.
int hardware_init()
{
    // Crank up the speed
    uLCD.baudrate(3000000);
    pc.baud(115200);
        
    //Initialize pushbuttons
    button1.mode(PullUp); 
    button2.mode(PullUp);
    button3.mode(PullUp);
    acc.activate();
    
    return ERROR_NONE;
}

GameInputs read_inputs() 
{
   
    // titlting the board down = -x
    // titlting the board right = -y 
    in.b1=button1;          // far left
    in.b2=button2;          // right of far left
    in.b3=button3;          // second to the right 
    
    acc.readXGravity(&in.ay);
    acc.readYGravity(&in.ax);
    acc.readZGravity(&in.az);
    //pc.printf("xAccel: %f   yAccel: %f   zAccel: %f\n",in.ay,in.ax,in.az);
    return in;
}
