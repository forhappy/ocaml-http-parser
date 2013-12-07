# Ocaml-http-parser #

This is a http request/response parser for OCAML based on [joyent/http-parser](https://github.com/joyent/http-parser "joyent/http-parser"). It parses both requests and responses. The parser is designed to be used in performance HTTP applications. It does not make any syscalls nor allocations, it does not buffer data, it can be interrupted at anytime.

Features:

- No other dependencies.
- Handles persistent streams (keep-alive).
- Decodes chunked encoding.
- Upgrade support
- Defends against buffer overflow attacks.

The parser extracts the following information from HTTP messages:

- Header fields and values
- Content-Length
- Request method
- Response status code
- Transfer-Encoding
- HTTP version
- Request URL
- Message body

# Usage #

One `Http_parser` object is used per TCP connection. Initialize the object using `Http_parser.init()` and set the callbacks. That might look something like this for a request parser:

    let settings = {
        on_message_begin = on_message_begin_cb;
        on_url = on_url_cb;
        on_status_complete = on_status_complete_cb;
        on_header_field = on_header_field_cb;
        on_header_value = on_header_value_cb;
        on_headers_complete = on_headers_complete_cb;
        on_body = on_body_cb;
        on_message_complete = on_message_complete_cb
    }
    let p = init settings HTTP_REQUEST

When data is received on the socket execute the parser by calling `Http_parser.execute()`  and check for errors 

    let request = "GET /demo HTTP/1.1\r\nUpgrade: WebSocket\r\nConnection:Upgrade\r\nHost: example.com\r\nOrigin:http://example.com\r\nWebSocket-Protocol: sample\r\n"
    let nparsed = execute p request;

## Parse HTTP Request ##

    open Http_parser
    
    let on_message_begin_cb http_parser = 0
    let on_status_complete_cb http_parser = 0
    let on_headers_complete_cb http_parser = 0
    let on_message_complete_cb http_parser = 0
    
    let on_url_cb http_parser data len =
        print_string ("on_url_cb: " ^ data ^ "\n"); 0
    
    let on_header_field_cb http_parser data len =
        print_string ("on_header_field_cb: " ^ data ^ "\n"); 0
    
    let on_header_value_cb http_parser data len =
        print_string ("on_header_value_cb: " ^ data ^ "\n"); 0
    
    let on_body_cb http_parser data len =
        print_string ("on_body_cb: " ^ data ^ "\n"); 0
    
    let settings = {
        on_message_begin = on_message_begin_cb;
        on_url = on_url_cb;
        on_status_complete = on_status_complete_cb;
        on_header_field = on_header_field_cb;
        on_header_value = on_header_value_cb;
        on_headers_complete = on_headers_complete_cb;
        on_body = on_body_cb;
        on_message_complete = on_message_complete_cb
    }
    let request = "GET /demo HTTP/1.1\r\nUpgrade: WebSocket\r\nConnection:Upgrade\r\nHost: example.com\r\nOrigin:http://example.com\r\nWebSocket-Protocol: sample\r\n"
    
    let p = init settings HTTP_REQUEST
    
    let nparsed = execute p request;

## Parse HTTP Response ##

    open Http_parser
    
    let on_message_begin_cb http_parser = 0
    let on_status_complete_cb http_parser = 0
    let on_headers_complete_cb http_parser = 0
    let on_message_complete_cb http_parser = 0
    
    let on_url_cb http_parser data len =
        print_string ("on_url_cb: " ^ data ^ "\n"); 0
    
    let on_header_field_cb http_parser data len =
        print_string ("on_header_field_cb: " ^ data ^ "\n"); 0
    
    let on_header_value_cb http_parser data len =
        print_string ("on_header_value_cb: " ^ data ^ "\n"); 0
    
    let on_body_cb http_parser data len =
        print_string ("on_body_cb: " ^ data ^ "\n"); 0
    
    let settings = {
        on_message_begin = on_message_begin_cb;
        on_url = on_url_cb;
        on_status_complete = on_status_complete_cb;
        on_header_field = on_header_field_cb;
        on_header_value = on_header_value_cb;
        on_headers_complete = on_headers_complete_cb;
        on_body = on_body_cb;
        on_message_complete = on_message_complete_cb
    }
    
    let response = "HTTP/1.0 200 OK\r\nDate: Fri, 31 Dec 1999 23:59:59 GMT\r\nContent-Type: text/html\r\nContent-Length: 1354\r\n\r\n<html><body><h1>Happy HTTP!</h1></body></html>"
    
    let p = init settings HTTP_RESPONSE
    
    let nparsed = execute p response;

# Callbacks #

During the `Http_parser.execute()` call, the callbacks set in
`http_parser_settings` will be executed. 

There are two types of callbacks:

1. Data callbacks: `type http_data_cb = http_parser -> string -> int -> int`:  Callbacks: (requests only) `on_uri`, (common) `on_header_field`, `on_header_value`, `on_body`;
2. Notification callbacks: `type http_cb = http_parser -> int`: Callbacks: `on_message_begin`, `on_status_complete`, `on_headers_complete`, `on_message_complete`.

    

   

Callbacks must return 0 on success. Returning a non-zero value indicates
error to the parser, making it exit immediately.

In case you parse HTTP message in chunks (i.e. `read()` request line
from socket, parse, read half headers, parse, etc) your data callbacks
may be called more than once. Http-parser guarantees that data pointer is only
valid for the lifetime of callback. You can also `read()` into a heap allocated
buffer to avoid copying memory around if this fits your application.

Reading headers may be a tricky task if you read/parse headers partially.
Basically, you need to remember whether last header callback was field or value
and apply following logic:

    (on_header_field and on_header_value shortened to on_h_*)
     ------------------------ ------------ --------------------------------------------
    | State (prev. callback) | Callback   | Description/action                         |
     ------------------------ ------------ --------------------------------------------
    | nothing (first call)   | on_h_field | Allocate new buffer and copy callback data |
    |                        |            | into it                                    |
     ------------------------ ------------ --------------------------------------------
    | value                  | on_h_field | New header started.                        |
    |                        |            | Copy current name,value buffers to headers |
    |                        |            | list and allocate new buffer for new name  |
     ------------------------ ------------ --------------------------------------------
    | field                  | on_h_field | Previous name continues. Reallocate name   |
    |                        |            | buffer and append callback data to it      |
     ------------------------ ------------ --------------------------------------------
    | field                  | on_h_value | Value for current header started. Allocate |
    |                        |            | new buffer and copy callback data to it    |
     ------------------------ ------------ --------------------------------------------
    | value                  | on_h_value | Value continues. Reallocate value buffer   |
    |                        |            | and append callback data to it             |
     ------------------------ ------------ --------------------------------------------

# Parsing URLs #

A simplistic zero-copy URL parser is provided as `Http_parser.parse_url str_url is_connected`. Users of this library may wish to use it to parse URLs constructed from consecutive `on_url` callbacks.
