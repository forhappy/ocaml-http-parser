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

