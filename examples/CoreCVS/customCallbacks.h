//
// Created by dio on 7/26/20.
//

#ifndef LIBEVENTAPP_CUSTOMCALLBACKS_H
#define LIBEVENTAPP_CUSTOMCALLBACKS_H

#include "../../LibEventServer.h"

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const short BUF_LEN = 41;
const char RESPONCE[BUF_LEN] = "<H1>Hello there, drones' users</H1><BR/>";
const char *SERVER_NAME = "Simple HTTP Server";

const char *HTML_TOP = "<html><head><title>%s title</title><style>#menu {display: grid;color: red;width: 100%;height: 100px;background-color: #abcdef;grid-template-columns: auto repeat(3, 200px) 100px;}#menu button {margin: 5%;}#menuOption1 { grid-column: 2; }#menuOption2 { grid-column: 3; }#menuOption3 { grid-column: 4; }</style></head><body><div id=\"menu\"><button id=\"menuOption1\">Some button</button><button id=\"menuOption2\">Some image</button><button id=\"menuOption3\">Some text</button></div><div id=\"content\"></div>";
const char *HTML_BOTTOM = "<script>const buttonAJAX = document.getElementById('menuOption1');buttonAJAX.addEventListener('click', loadAJAX);function loadAJAX() {var xhr = new XMLHttpRequest();xhr.open('GET', '/AJAX_request', true);xhr.onload = function() {if (this.status == 200) console.log(this.responseText);};};</script></body></html>";

const char *HTML_BODY1 = "<canvas width=\"255px\" height=\"255px\" id=\"Canvas\"></canvas><script>;const canvas=document.getElementById('Canvas');const context=canvas.getContext('2d');const width=canvas.width;const height=canvas.height;const imagedata=context.createImageData(width,height);function createImage() {var pixelindex = 0; var pictureStr = \"";
const char *HTML_BODY2 = "\"; var arr = pictureStr.split(\" \"); console.log(arr); for (var x=4; x<width*height*3+4; x+=3) { imagedata.data[pixelindex] = arr[x]; imagedata.data[pixelindex+1] = arr[x+1]; imagedata.data[pixelindex+2] = arr[x+2]; imagedata.data[pixelindex+3] = 255; /*console.log(`R=${arr[x]}, G=${arr[x + 1]}, B=${arr[x + 2]}`);*/ pixelindex+=4; } }; function main(t){/*window.requestAnimationFrame(main);*/createImage();context.putImageData(imagedata,0,0)};main(0);</script>";

LibEventServer serv;

void on_get_index(struct evhttp_request *req, void *arg)
{
    // Create responce buffer
    struct evbuffer *evb = evbuffer_new();
    if (!evb) return; // No pointer returned

    evbuffer_add_printf(evb, HTML_TOP, SERVER_NAME);
    evbuffer_add(evb, RESPONCE, BUF_LEN);

    int fd;
    struct stat stbuf;

    if (((fd = open("picture.ppm", O_RDONLY)) < 0) || (fstat(fd, &stbuf) < 0))
    {
        evbuffer_add_printf(evb, "Can't get image from drone (ERR_CODE 1)\n");
        evbuffer_add_printf(evb, "%s", HTML_BOTTOM);
        evhttp_send_reply(req, HTTP_OK, "OK", evb);
        evbuffer_free(evb);
        if (fd > -1) close(fd);
        return;
    }

    evbuffer_add_printf(evb, "%s", HTML_BODY1);

    int total_read_bytes = 0;
    int read_bytes;
    while(total_read_bytes < stbuf.st_size)
    {
        read_bytes = evbuffer_read(evb, fd, stbuf.st_size);
        if (read_bytes < 0)
        {
            evbuffer_add_printf(evb, "Can't get image from drone (ERR_CODE 2)\n");
            evbuffer_add_printf(evb, "%s", HTML_BOTTOM);
            evhttp_send_reply(req, HTTP_OK, "OK", evb);
            evbuffer_free(evb);
            close(fd);
            return;
        }
        total_read_bytes += read_bytes;
    }
    evbuffer_add_printf(evb, "%s", HTML_BODY2);

    evbuffer_add_printf(evb, "Some text\n");
    evbuffer_add_printf(evb, "%s", HTML_BOTTOM);

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
    evbuffer_add_printf(evb, "%s", HTML_BOTTOM);
    evhttp_send_reply(req, HTTP_NOTFOUND, "NOT_FOUND", evb);
    evbuffer_free(evb);
}

#endif //LIBEVENTAPP_CUSTOMCALLBACKS_H
