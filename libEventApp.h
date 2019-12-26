//
// Created by dio on 06.12.2019.
//

#ifndef LIBEVENTAPP_LIBEVENTAPP_H
#define LIBEVENTAPP_LIBEVENTAPP_H

#include <iostream>
#include <getopt.h>
#include <sys/stat.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>

#include "libEventApp.h"
#include <evhttp.h>

struct evhttp *libsrvr;
struct event_base *libbase;

struct _options {
    int port;
    char* addr;
    int verbose;
} options;

const short  BUF_LEN           = 41;
const char   RESPONCE[BUF_LEN] = "<H1>Hello there, drones' users</H1><BR/>";
const char * SERVER_NAME       = "Simple HTTP Server";

const int   DEFAULT_PORT = 8040;
const char* DEFAULT_IP_ADDRESS = "0.0.0.0";

#endif //LIBEVENTAPP_LIBEVENTAPP_H
