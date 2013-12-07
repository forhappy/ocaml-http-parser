type http_parser
type http_method =
    DELETE
  | GET
  | HEAD
  | POST
  | PUT
  | CONNECT
  | OPTIONS
  | TRACE
  | COPY
  | LOCK
  | MKCOL
  | MOVE
  | PROPFIND
  | PROPPATCH
  | SEARCH
  | UNLOCK
  | REPORT
  | MKACTIVITY
  | CHECKOUT
  | MERGE
  | MSEARCH
  | NOTIFY
  | SUBSCRIBE
  | UNSUBSCRIBE
  | PATCH
  | PURGE
type http_errno =
    OK
  | CB_message_begin
  | CB_status_complete
  | CB_url
  | CB_header_field
  | CB_header_value
  | CB_headers_complete
  | CB_body
  | CB_message_complete
  | INVALID_EOF_STATE
  | HEADER_OVERFLOW
  | CLOSED_CONNECTION
  | INVALID_VERSION
  | INVALID_STATUS
  | INVALID_METHOD
  | INVALID_URL
  | INVALID_HOST
  | INVALID_PORT
  | INVALID_PATH
  | INVALID_QUERY_STRING
  | INVALID_FRAGMENT
  | LF_EXPECTED
  | INVALID_HEADER_TOKEN
  | INVALID_CONTENT_LENGTH
  | INVALID_CHUNK_SIZE
  | INVALID_CONSTANT
  | INVALID_INTERNAL_STATE
  | STRICT
  | PAUSED
  | UNKNOWN
type http_parser_type = HTTP_REQUEST | HTTP_RESPONSE | HTTP_BOTH
type http_data_cb = http_parser -> string -> int -> int
type http_cb = http_parser -> int
type http_parser_settings = {
  on_message_begin : http_cb;
  on_url : http_data_cb;
  on_status_complete : http_cb;
  on_header_field : http_data_cb;
  on_header_value : http_data_cb;
  on_headers_complete : http_cb;
  on_body : http_data_cb;
  on_message_complete : http_cb;
}
type http_parser_url = {
  schema : string;
  host : string;
  port : string;
  path : string;
  query : string;
  fragment : string;
  userinfo : string;
}
external version : unit -> int * int * int = "caml_http_parser_version"
external init : http_parser_settings -> http_parser_type -> http_parser
  = "caml_http_parser_init"
external execute : http_parser -> string -> int = "caml_http_parser_execute"
external pause : http_parser -> int -> unit = "caml_http_parser_pause"
external body_is_final : http_parser -> int = "caml_http_body_is_final"
external should_keep_alive : http_parser -> int
  = "caml_http_should_keep_alive"
external method_code : http_parser -> http_method = "caml_http_method_code"
external http_method_str : http_method -> string = "caml_http_method_str"
external errno_code : http_parser -> http_errno = "caml_http_errno_code"
external http_errno_name : http_errno -> string = "caml_http_errno_name"
external http_errno_description : http_errno -> string
  = "caml_http_errno_description"
external status_code : http_parser -> int = "caml_http_status_code"
external is_upgrade : http_parser -> int = "caml_http_is_upgrade"
external parse_url : string -> int -> http_parser_url
  = "caml_http_parser_parse_url"
