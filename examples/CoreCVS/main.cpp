
#include "../../LibEventServer.h"
#include "customCallbacks.h"

int main(int argc, char **argv) {
    server = LibEventServer();    // Creating an instance of LibEventServer
    server.options.verbose = 1;   // Configuring port, IP and other available options
    server.options.port = 8040;
    server.setup();               // Starting server with current configuration
    server.set_callback("/",on_get_index);                  // Adding handlers
    server.set_callback("/image_request", on_image_request);// for different routes
    server.set_callback("/stats_request", on_stats_request);// for different routes
    server.set_default_callback(on_other_requests);

    while(true)
    {
        // Performing all necessary activities in the main loop
        // Calling PROCESS_REQUESTS function to respond to pending HTTP requests
        server.process_requests();
    }
}
