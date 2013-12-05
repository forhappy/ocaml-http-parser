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

