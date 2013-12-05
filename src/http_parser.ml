type http_parser

type http_method = DELETE | GET | HEAD | POST | PUT | CONNECT
  | OPTIONS | TRACE | COPY | LOCK | MKCOL | MOVE | PROPFIND
  | PROPPATCH | SEARCH | UNLOCK | REPORT | MKACTIVITY | CHECKOUT
  | MERGE | MSEARCH | NOTIFY | SUBSCRIBE | UNSUBSCRIBE | PATCH
  | PURGE

type http_parser_type = HTTP_REQUEST | HTTP_RESPONSE | HTTP_BOTH

type http_data_cb = http_parser -> string -> int -> int
type http_cb = http_parser -> int

type http_parser_settings = {
  on_message_begin:    http_cb;
  on_url:              http_data_cb;
  on_status_complete:  http_cb;
  on_header_field:     http_data_cb;
  on_header_value:     http_data_cb;
  on_headers_complete: http_cb;
  on_body:             http_data_cb;
  on_message_complete: http_cb
}

external version:
     unit
  -> int * int * int = "caml_http_parser_version"

external init:
     http_parser_settings
  -> http_parser_type
  -> http_parser = "caml_http_parser_init"

external execute:
     http_parser
  -> string
  -> int = "caml_http_parser_execute"
