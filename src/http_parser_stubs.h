#ifndef _HTTP_PARSER_STUBS_H
#define _HTTP_PARSER_STUBS_H

/**
 * The ocaml_http_parser_t wraps a http_parser pointer.
 */
typedef struct ocaml_http_parser_s_ {
  http_parser *parser;
} ocaml_http_parser_t;

#endif // _HTTP_PARSER_STUBS_H
