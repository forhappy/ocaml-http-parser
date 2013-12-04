#ifndef _HTTP_PARSER_STUBS_H
#define _HTTP_PARSER_STUBS_H

#include <caml/mlvalues.h>

#include "http_parser/http_parser.h"

typedef struct caml_http_parser_settings_s_ {
  value on_message_begin;
  value on_url;
  value on_status_complete;
  value on_header_field;
  value on_header_value;
  value on_headers_complete;
  value on_body;
  value on_message_complete;
} caml_http_parser_settings_t;

/**
 * The ocaml_http_parser_t wraps a http_parser pointer.
 */
typedef struct caml_http_parser_s_ {
  http_parser *parser;
  http_parser_settings *native_settings;
} caml_http_parser_t;

#endif // _HTTP_PARSER_STUBS_H
