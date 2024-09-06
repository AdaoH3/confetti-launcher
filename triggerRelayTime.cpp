#include <iostream>
#include <pigpio.h>
#include <unistd.h>
#include <cmath>

const int RELAY_PIN = 27; // Change this to your relay pin

int calcMs(int mili)
{
    float tempMili = mili / 1000.0f; //In Seconds
    if(tempMili > 900)
    {
        tempMili = pow((1 + 0.0004069762), tempMili) + 8.26;
    }
    else if(tempMili > 300)
    {
        tempMili = ((1/90.0f) * tempMili - .3);
    }
    else
    {
        tempMili = 0.000032222f * pow(tempMili, 2) + 0.05f;
    }

    return round(tempMili * 1000.0);
}

void startRelay(int receivedTimeSec) {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialisation failed." << std::endl;
        return;
    }

    gpioSetMode(RELAY_PIN, PI_OUTPUT);

    int receivedTimeMs = receivedTimeSec * 1000; // Convert seconds to milliseconds
    int triggerMs = calcMs(receivedTimeMs);



    bool toggleState = false;

    //std::cout << "Starting relay with receivedTime: " << receivedTimeSec << " seconds" << std::endl;

    // Adjust the rate to gradually decrease until it reaches 100ms at around 5000ms (5 seconds) remaining
    while (receivedTimeMs >= 0 )
    {
        triggerMs = calcMs(receivedTimeMs);
        gpioWrite(RELAY_PIN, toggleState);
       // std::cout << "Toggled relay to state: " << toggleState << ", waiting for " << triggerMs << std::endl;
        usleep(triggerMs * 1000);
        receivedTimeMs -= triggerMs;
        toggleState =!toggleState;
    }
    // Toggle relay 100 more times at 100ms interval
    for (int i = 0; i < 10; i++) {
        gpioWrite(RELAY_PIN, toggleState);
        std::cout << "Toggled relay to state: " << toggleState << ", waiting for 100 ms" << std::endl;
        usleep(100 * 1000); // 100 ms
        toggleState = !toggleState;
    }

    gpioWrite(RELAY_PIN, 1); // Turn off relay at the end
    gpioTerminate();
    std::cout << "Relay control finished." << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <receivedTimeSec>" << std::endl;
        return 1;
    }

    int receivedTimeSec = std::stoi(argv[1]);
    startRelay(receivedTimeSec);

    return 0;
}
