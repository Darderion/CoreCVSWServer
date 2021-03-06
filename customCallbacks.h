//
// Created by dio on 7/26/20.
//

#ifndef LIBEVENTAPP_CUSTOMCALLBACKS_H
#define LIBEVENTAPP_CUSTOMCALLBACKS_H

#include "Libs/ImageGenerator.h"
#include "Libs/StatsGenerator.h"
#include "LibEventServer.h"

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <chrono>

#include <iterator>
#include <vector>
#include <sstream>

#include "Libs/base64.cpp"

LibEventServer server;

void on_get_index(struct evhttp_request *req, void *arg)
{
    evbuffer *evb = evbuffer_new(); // Creating a response buffer
    if (!evb) return;               // No pointer returned

    // Access html using file stream
    std::ifstream fin("Pages/index.html", std::ios::in | std::ios::binary);
    std::ostringstream oss;
    oss << fin.rdbuf();
    std::string data(oss.str());

    // Add image to buffer as base64 string
    evbuffer_add_printf(evb, "%s", data.c_str());

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void on_image_request(struct evhttp_request *req, void *arg)
{
#ifdef display_performance_measurements
    auto start = std::chrono::steady_clock::now();
#endif

    const char *img_name = generateImage();
    evbuffer *evb = evbuffer_new(); // Creating a response buffer
    if (!evb) return;               // No pointer returned

    // Access image using file stream
    std::ifstream fin(img_name, std::ios::in | std::ios::binary);
    std::ostringstream oss;
    oss << fin.rdbuf();
    std::string data(oss.str());

    // Add image to buffer as base64 string
    evbuffer_add_printf(evb, "%s", base64_encode(data).c_str());

    evhttp_add_header(req->output_headers, "Content-Type", "image/jpeg");

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);

#ifdef display_performance_measurements
    auto finish = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
#endif
}

void on_get_stats_request(struct evhttp_request *req, void *arg)
{
    evbuffer *evb = evbuffer_new(); // Creating a response buffer
    if (!evb) return;               // No pointer returned

    evbuffer_add_printf(evb, "Parameters_Changed@%i$%i$%i@%i$%i$%i",
                        getRoll(), getPitch(), getYaw(),
                        getParam1(), getParam2(), getParam3());

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void on_change_stats_request(struct evhttp_request *req, void *arg)
{
    evbuffer *evb = evbuffer_new(); // Creating a response buffer
    if (!evb) return;               // No pointer returned

    char *param;
    char *value;

    strtok(req->uri, "?");

    if ((param = strtok(NULL, "?")) != nullptr) {
        if ((value = strtok(NULL, "?")) != nullptr) {
            switch(atoi(param))
            {
                case 1: setParam1(atoi(value)); break;
                case 2: setParam2(atoi(value)); break;
                case 3: setParam3(atoi(value)); break;
                default:
                    evhttp_send_reply(req, HTTP_BADREQUEST, "Wrong parameter", evb);
                    evbuffer_free(evb);
                    return;
            }
            evhttp_send_reply(req, HTTP_OK, "OK", evb);
            evbuffer_free(evb);
            return;
        }
    }

    evhttp_send_reply(req, HTTP_BADREQUEST, "No parameters", evb);
    evbuffer_free(evb);
}

void on_other_requests(struct evhttp_request * req, void *arg)
{
    evbuffer *evb = evbuffer_new(); // Creating a response buffer
    if (!evb) return;               // No pointer returned

    evbuffer_add_printf(evb, "Page not found\n404\n");
    evhttp_send_reply(req, HTTP_NOTFOUND, "NOT_FOUND", evb);
    evbuffer_free(evb);
}

#endif //LIBEVENTAPP_CUSTOMCALLBACKS_H
