//
// Created by dio on 18.12.2019.
//

#ifndef LIBEVENTAPP_CUSTOMCALLBACKS_H
#define LIBEVENTAPP_CUSTOMCALLBACKS_H

#include "../../LibEventServer.h"

const short BUF_LEN = 41;
const char RESPONCE[BUF_LEN] = "<H1>Hello there, drones' users</H1><BR/>";
const char *SERVER_NAME = "Simple HTTP Server";

const char *HTML_TOP = "<html><head><title>%s title</title><style>#menu {display: grid;color: red;width: 100%;height: 100px;background-color: #abcdef;grid-template-columns: auto repeat(3, 200px) 100px;}#menu button {margin: 5%;}#menuOption1 { grid-column: 2; }#menuOption2 { grid-column: 3; }#menuOption3 { grid-column: 4; }</style></head><body><div id=\"menu\"><button id=\"menuOption1\">Some button</button><button id=\"menuOption2\">Some image</button><button id=\"menuOption3\">Some text</button></div><div id=\"content\"></div>";
const char *HTML_BOTTOM = "<script>const buttonAJAX = document.getElementById('menuOption1');buttonAJAX.addEventListener('click', loadAJAX);function loadAJAX() {var xhr = new XMLHttpRequest();xhr.open('GET', '/AJAX_request', true);xhr.onload = function() {if (this.status == 200) console.log(this.responseText);};};</script></body></html>";

LibEventServer serv;
int step = 0;
int requests_number = 0;

void on_get_index(struct evhttp_request *req, void *arg)
{
    // Create responce buffer
    struct evbuffer *evb = evbuffer_new();
    if (!evb) return; // No pointer returned
    requests_number++;

    evbuffer_add_printf(evb, HTML_TOP, SERVER_NAME);
    evbuffer_add(evb, RESPONCE, BUF_LEN);
    evbuffer_add_printf(evb, "Current number that increments on server = %i (Request number %i)\n", step, requests_number);
    evbuffer_add_printf(evb, HTML_BOTTOM);

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void on_AJAX(struct evhttp_request *req, void *arg)
{
    struct evbuffer *evb = evbuffer_new();
    if (!evb) return;

    evbuffer_add_printf(evb, "Woooooooooooo");

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void on_other_requests(struct evhttp_request * req, void *arg)
{
    evbuffer *evb = evbuffer_new();
    if (!evb) return;

    evbuffer_add_printf(evb, HTML_TOP, SERVER_NAME);
    evbuffer_add_printf(evb, "Page not found\n404\n");
    evbuffer_add_printf(evb, HTML_BOTTOM);
    evhttp_send_reply(req, HTTP_NOTFOUND, "NOT_FOUND", evb);
    evbuffer_free(evb);
}

#endif //LIBEVENTAPP_CUSTOMCALLBACKS_H
