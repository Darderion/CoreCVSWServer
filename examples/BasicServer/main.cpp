
#include <iostream>
#include "../../LibEventServer.h"
#include "customCallbacks.h"

// This example illustrates how LibEventServer can run in the same thread
// as main program.

int main(int argc, char **argv) {
    std::cout << "Init" << std::endl;

    serv = LibEventServer();    // Creating an instance of LibEventServer
    serv.options.verbose = 1;   // Configuring port, IP and other available options
    serv.setup();               // Starting server with current configuration
    serv.set_callback("/",on_get_index);        // Adding handlers
    serv.set_callback("/AJAX_request", on_AJAX);// for different routes
    serv.set_default_callback(on_other_requests);

    while(true)
    {
        // Performing all necessary activities in the main loop
        step = (step + 1) % 1000000;
        // Calling PROCESS_REQUESTS function to respond to pending HTTP requests
        serv.process_requests();
    }

    return 0;
}
