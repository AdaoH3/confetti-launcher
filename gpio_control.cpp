#include <iostream>
#include <pigpio.h>
#include <string>

void startRelayTicks() {
    // Implement relay tick logic here
    std::cout << "startRelayTicks executed" << std::endl;
}

void startDrill() {
    // Implement drill start logic here
    gpioSetMode(22, PI_OUTPUT);
    gpioWrite(22, 0);
    gpioDelay(6000000);
    gpioWrite(22, 1);
    std::cout << "startDrill executed" << std::endl;
}

void stopDrill() {
    // Implement drill stop logic here
    std::cout << "stopDrill executed" << std::endl;
}

void setGPIOPin(int pin, int status) {
    gpioSetMode(pin, PI_OUTPUT);
    //For some reason its fucking switched on this library for writing to a pin so (0 = ON, 1 = OFF) So instead of dealing with that shit just made another variable to swap
    int legitVersion = status;

    if(status == 0)
    {
        legitVersion = 1;
    }
    else
    {
        legitVersion = 0;
    }

    gpioWrite(pin, legitVersion);
    std::cout << "setGPIOPin executed: pin " << pin << " set to " << status << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " function [pin] [action]" << std::endl;
        return 1;
    }

   gpioInitialise();

    std::string function = argv[1];
    if (function == "startRelayTicks") {
        startRelayTicks();
    } else if (function == "startDrill") {
        startDrill();
    } else if (function == "stopDrill") {
        stopDrill();
    } else if (function == "setGPIOPin") {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " setGPIOPin pin action" << std::endl;
            return 1;
        }
        int pin = std::stoi(argv[2]);
        int action = std::stoi(argv[3]);
        setGPIOPin(pin, action);
    } else {
        std::cerr << "Invalid function" << std::endl;
        return 1;
    }
    
    gpioTerminate();
    return 0;
}
