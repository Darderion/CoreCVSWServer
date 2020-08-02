//
// Created by dio on 7/26/20.
//

#ifndef LIBEVENTAPP_CUSTOMCALLBACKS_H
#define LIBEVENTAPP_CUSTOMCALLBACKS_H

#include "ImageGenerator.h"
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

const short BUF_LEN = 41;
const char RESPONCE[BUF_LEN] = "<H1>Hello there, drones' users</H1><BR/>";
const char *SERVER_NAME = "Simple HTTP Server";
const char *HTML_TOP = "<html><head><title>%s title</title><style>#menu {display: grid;color: red;width: 100%;height: 100px;background-color: #abcdef;grid-template-columns: auto repeat(3, 200px) 100px;}#menu button {margin: 5%;}#menuOption1 { grid-column: 2; }#menuOption2 { grid-column: 3; }#menuOption3 { grid-column: 4; }</style></head>";
const char *HTML_BODY = "<body><div id='menu'><button id='menuOption1'>Some button</button><button id='menuOption2'>Some image</button><button id='menuOption3'>Some text</button></div><div id='content'></div><canvas id='Canvas' width='255' height='255'></canvas><img id='img'>";
const char *HTML_BOTTOM = "<script>$(document).ready(function() { const buttonAJAX = document.getElementById('menuOption1');buttonAJAX.addEventListener('click', loadAJAX);function loadAJAX() { var xhr = new XMLHttpRequest();xhr.open('GET', '/AJAX_request', true);xhr.onload = function() {if (this.status == 200) console.log(this.responseText); else console.log(this.status); };};"
                          // "setInterval(_=>{jQuery.get('AJAX_request', null, data => { drawCameraImage(data)}); }, 50);"
                          "var performance = [0, 0, 0, 0, 0];"
                          "function getCameraImage() { jQuery.get('AJAX_request', null, data => { $('#img').attr('src', 'data:image/jpg;base64,'+data);"
                          "performance[1] = new Date(); getCameraImage(); performance[2] = new Date(); fps++; drawCameraImage(data); performance[3] = new Date(); console.log('Request received after '+(performance[1] - performance[0])+' ms and processed after '+(performance[2] - performance[0]) + ' ms (Drawn after '+(performance[3]-performance[2])+' ms)'); performance[0] = performance[3]; } ); }"
                          "var fps = 0; setInterval(_ => { console.log(fps); fps = 0; }, 1000);"
                          "getCameraImage();"
                          "const canvas=document.getElementById('Canvas'); const context=canvas.getContext('2d');const width=canvas.width;const height=canvas.height;const imagedata=context.createImageData(width,height);"
                          "var pixelindex = 0; function createImage(str) { pixelindex = 0; var arr = str.split(' '); arr.splice(0, 4); const max_image_size = width*height*3; for (var x=0; x<max_image_size; x+=3) { imagedata.data[pixelindex] = arr[x]; imagedata.data[pixelindex+1] = arr[x+1]; imagedata.data[pixelindex+2] = arr[x+2]; imagedata.data[pixelindex+3] = 255; pixelindex+=4; } };"
                          "function drawCameraImage(str){ createImage(str); context.putImageData(imagedata,0,0)}; });</script>"
                          "</body></html>";
const char *JQueryLink = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>";

LibEventServer server;

void on_get_index(struct evhttp_request *req, void *arg)
{
    // Create responce buffer
    struct evbuffer *evb = evbuffer_new();
    if (!evb) return; // No pointer returned

    evbuffer_add_printf(evb, HTML_TOP, SERVER_NAME);
    evbuffer_add(evb, RESPONCE, BUF_LEN);

    int fd;
    struct stat stbuf;

    if (((fd = open(img_filename, O_RDONLY)) < 0) || (fstat(fd, &stbuf) < 0))
    {
        evbuffer_add_printf(evb, "Can't get image from drone (ERR_CODE 1)\n");
        evbuffer_add_printf(evb, "%s", HTML_BOTTOM);
        evhttp_send_reply(req, HTTP_OK, "OK", evb);
        evbuffer_free(evb);
        if (fd > -1) close(fd);
        return;
    }

    evbuffer_add_printf(evb, "%s", HTML_BODY);

    evbuffer_add_printf(evb, "Some text\n");
    evbuffer_add_printf(evb, "%s", JQueryLink);
    evbuffer_add_printf(evb, "%s", HTML_BOTTOM);

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
    close(fd);
}

void on_AJAX(struct evhttp_request *req, void *arg)
{
#ifdef display_performance_measurements
    auto start = std::chrono::steady_clock::now();
#endif

    struct evbuffer *evb = evbuffer_new();
    int fd;
    struct stat stbuf;

    if (!evb) return;

    if (((fd = open(img_filename, O_RDONLY)) < 0) || (fstat(fd, &stbuf) < 0))
    {
        evbuffer_add_printf(evb, "Can't get image from drone (ERR_CODE 1)\n");
        evbuffer_add_printf(evb, "%s", HTML_BOTTOM);
        evhttp_send_reply(req, HTTP_OK, "OK", evb);
        evbuffer_free(evb);
        if (fd > -1) close(fd);
        return;
    }

    /*
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
     */

    std::ifstream fin(img_filename, std::ios::in | std::ios::binary);
    std::ostringstream oss;
    oss << fin.rdbuf();
    std::string data(oss.str());

    evbuffer_add_printf(evb, "%s", base64_encode(data).c_str());

    // if (evbuffer_add_file(evb, fd, 0, stbuf.st_size) == -1) std::cout << "Error while reading a file" << std::endl;

    evhttp_add_header(req->output_headers, "Content-Type", "image/jpeg");

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
    close(fd);

#ifdef display_performance_measurements
    auto finish = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
#endif
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
