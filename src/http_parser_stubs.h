#ifndef _HTTP_PARSER_STUBS_H
#define _HTTP_PARSER_STUBS_H

#include "http_parser/http_parser.h"

/**
 * The ocaml_http_parser_t wraps a http_parser pointer.
 */
typedef struct caml_http_parser_s_ {
  http_parser *parser;
  http_parser_settings *settings;
} caml_http_parser_t;

#endif // _HTTP_PARSER_STUBS_H
