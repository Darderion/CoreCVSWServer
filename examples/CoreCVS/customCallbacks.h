//
// Created by dio on 7/26/20.
//

#ifndef LIBEVENTAPP_CUSTOMCALLBACKS_H
#define LIBEVENTAPP_CUSTOMCALLBACKS_H

#include "ImageGenerator.h"
#include "RPYTGenerator.h"
#include "../../LibEventServer.h"

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

#include "base64.cpp"

const char *RESPONCE = "<H1>Hello there, drones' users</H1><BR/>";
const char *SERVER_NAME = "Simple HTTP Server";
const char *HTML_TOP = "<html><head><title>%s title</title><style>#menu {display: grid;color: red;width: 100%;height: 100px;background-color: #abcdef;grid-template-columns: auto repeat(3, 200px) 100px;}#menu button {margin: 5%;}#menuOption1 { grid-column: 2; }#menuOption2 { grid-column: 3; }#menuOption3 { grid-column: 4; }</style></head>";
const char *HTML_BODY = "<body><div id='menu'><button id='menuOption1'>Some button</button><button id='menuOption2'>Some image</button><button id='menuOption3'>Some text</button></div><div id='content'></div><img id='img' style='object-fit: cover; width:800px;'><div id='RollDiv'></div><div id='PitchDiv'></div><div id='YawDiv'></div>";
const char *HTML_BOTTOM = "<script>$(document).ready(function() { const buttonAJAX = document.getElementById('menuOption1');buttonAJAX.addEventListener('click', loadAJAX);function loadAJAX() { var xhr = new XMLHttpRequest();xhr.open('GET', '/AJAX_request', true); xhr.onload = function() {if (this.status == 200) console.log(this.responseText); else console.log(this.status); };};"
                          "var performance = [0, 0, 0];"
                          "function getCameraImage() { jQuery.get('image_request', null, data => { $('#img').attr('src', 'data:image/png;base64,'+data); performance[1] = new Date(); getCameraImage(); performance[2] = new Date(); fps++;"
#ifdef display_performance_in_browser
                          "console.log('Image request received after '+(performance[1] - performance[0])+' ms and processed after '+(performance[2] - performance[0]) + ' ms'); performance[0] = performance[2];"
#endif
                          "} ); }; function getStats() { jQuery.get('stats_request', null, data => { const arr = data.split('$'); $('#RollDiv').text('Roll...'+arr[0]); $('#PitchDiv').text('Pitch..'+arr[1]); $('#YawDiv').text('Yaw....'+arr[2]); getStats(); } ); }"
                          "var fps = 0; setInterval(_ => { console.log('FPS: '+fps); fps = 0; }, 1000); getCameraImage(); getStats();"
                          "});</script></body></html>";
const char *JQueryLink = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>";

LibEventServer server;

void on_get_index(struct evhttp_request *req, void *arg)
{
    evbuffer *evb = evbuffer_new(); // Creating a response buffer
    if (!evb) return;               // No pointer returned

    evbuffer_add_printf(evb, HTML_TOP, SERVER_NAME);
    evbuffer_add_printf(evb, "%s", RESPONCE);

    evbuffer_add_printf(evb, "%s", HTML_BODY);

    evbuffer_add_printf(evb, "Some text\n");
    evbuffer_add_printf(evb, "%s", JQueryLink);
    evbuffer_add_printf(evb, "%s", HTML_BOTTOM);

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

void on_stats_request(struct evhttp_request *req, void *arg)
{
    const char *img_name = generateImage();
    evbuffer *evb = evbuffer_new(); // Creating a response buffer
    if (!evb) return;               // No pointer returned

    evbuffer_add_printf(evb, "%i$%i$%i", getRoll(), getPitch(), getYaw());

    // evhttp_add_header(req->output_headers, "Content-Type", "image/jpeg");

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void on_other_requests(struct evhttp_request * req, void *arg)
{
    evbuffer *evb = evbuffer_new(); // Creating a response buffer
    if (!evb) return;               // No pointer returned

    evbuffer_add_printf(evb, HTML_TOP, SERVER_NAME);
    evbuffer_add_printf(evb, "Page not found\n404\n");
    evbuffer_add_printf(evb, "%s", HTML_BOTTOM);
    evhttp_send_reply(req, HTTP_NOTFOUND, "NOT_FOUND", evb);
    evbuffer_free(evb);
}

#endif //LIBEVENTAPP_CUSTOMCALLBACKS_H
