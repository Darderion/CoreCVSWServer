
#include <iostream>
#include "libEventApp.h"
#include "customCallbacks.h"

int main(int argc, char **argv) {
    using namespace std;

    options.port = DEFAULT_PORT;
    options.addr = const_cast<char *>(DEFAULT_IP_ADDRESS);
    options.verbose = 0;

    int opt;

    while ((opt = getopt(argc, argv, "p:vh")) != -1)
    {
        switch (opt)
        {
            case 'p':
                options.port = atoi(optarg);
                break;
            case 'v':
                options.verbose = 1;
                break;
            case 'h':
                cout << "Usage : ./main -p port -v[erbose] -h[elp]" << endl;
                exit(1);
        }
    }

    if ((libbase = event_base_new()) == nullptr)
    {
        cerr << "Event_base_new failed" << endl;
        return -1;
    }

    libsrvr = evhttp_new(libbase);
    evhttp_bind_socket(libsrvr, options.addr, options.port);

    evhttp_set_cb(libsrvr, "/", on_get_index, nullptr);
    evhttp_set_gencb(libsrvr, on_other_requests, nullptr);
    event_base_dispatch(libbase);

    getchar();

    return 0;
}
