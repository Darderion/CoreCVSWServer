
#include <iostream>
#include "../../LibEventServer.h"
#include "customCallbacks.h"
#include "ImageGenerator.h"

int main(int argc, char **argv) {
    server = LibEventServer();    // Creating an instance of LibEventServer
    server.options.verbose = 1;   // Configuring port, IP and other available options
    server.options.port = 8080;
    server.setup();               // Starting server with current configuration
    server.set_callback("/",on_get_index);        // Adding handlers
    server.set_callback("/AJAX_request", on_AJAX);// for different routes
    server.set_default_callback(on_other_requests);

    generateImage(step);

    while(true)
    {
        // Performing all necessary activities in the main loop
        step = (step + 1) % 1000000;
        generateImage(step);
        // Calling PROCESS_REQUESTS function to respond to pending HTTP requests
        server.process_requests();
    }

    return 0;
}
