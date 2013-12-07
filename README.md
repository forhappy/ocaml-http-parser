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
