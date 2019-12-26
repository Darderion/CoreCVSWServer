//
// Created by dio on 18.12.2019.
//

#ifndef LIBEVENTAPP_CUSTOMCALLBACKS_H
#define LIBEVENTAPP_CUSTOMCALLBACKS_H

#include "libEventApp.h"

void router(struct evhttp_request *req, void *arg)
{
    const char *uri = evhttp_request_get_uri(req);

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET)
    {
        evhttp_send_error(req, HTTP_BADREQUEST, nullptr);
    }
    else
    {
        evhttp_send_reply(req, HTTP_OK, nullptr, nullptr);
    }

    std::cout << uri << std::endl;
}

void on_get_index(struct evhttp_request *req, void *arg)
{

    // Create responce buffer
    struct evbuffer *evb = evbuffer_new();
    if (!evb) return; // No pointer returned

    evbuffer_add_printf(evb, "<HTML><HEAD><TITLE>%s Page</TITLE></HEAD><BODY>\n", SERVER_NAME);
    evbuffer_add(evb, RESPONCE, BUF_LEN);
    evbuffer_add_printf(evb, "Your request is <B>%s</B> from <B>%s</B>.<BR/>Your user agent is '%s'\n",
                        req->uri,
                        req->remote_host,
                        evhttp_find_header(req->input_headers, "User-Agent"));
    evbuffer_add_printf(evb, "</BODY></HTML>");

    evhttp_add_header(req->output_headers, "Server", SERVER_NAME);
    evhttp_add_header(req->output_headers, "Connection", "close");
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void on_other_requests(struct evhttp_request * req, void *arg)
{
    struct evbuffer *evb = evbuffer_new();
    if (!evb) return;

    evbuffer_add_printf(evb, "<HTML><HEAD><TITLE>%s Page</TITLE></HEAD><BODY>\n", SERVER_NAME);
    evbuffer_add(evb, RESPONCE, BUF_LEN);
    evbuffer_add_printf(evb, "Page not found\n404\n");
    evbuffer_add_printf(evb, "</BODY></HTML>");

    evhttp_add_header(req->output_headers, "Server", SERVER_NAME);
    evhttp_add_header(req->output_headers, "Connection", "close");
    evhttp_send_reply(req, HTTP_NOTFOUND, "NOT_FOUND", evb);
    evbuffer_free(evb);
}

#endif //LIBEVENTAPP_CUSTOMCALLBACKS_H
