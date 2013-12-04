/* Copyright (c) 2013 Fu Haiping <haipingf@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/signals.h>
#include <caml/threads.h>

#include "http_parser_stubs.h"

#include "http-parser/http_parser.h"

#define caml_http_parser_struct_val(v) \
  (*(caml_http_parser_t **)Data_custom_val(v))

static const enum http_parser_type HTTP_PARSER_TYPE_TABLE[] = {
  HTTP_REQUEST,
  HTTP_RESPONSE,
  HTTP_BOTH
};

static struct custom_operations caml_http_parser_struct_ops = {
  "org.ocaml.http_parser",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

static enum http_parser_type
caml_http_parser_type_ml2c(value v)
{
  CAMLparam1(v);
  return HTTP_PARSER_TYPE_TABLE[Long_val(v)];
}

static value
caml_copy_http_parser(http_parser *parser,
                      http_parser_settings *settings)
{
  CAMLparam0();
  CAMLlocal1(caml_parser);

  caml_http_parser_t *native_parser = (caml_http_parser_t *)
      malloc(sizeof(caml_http_parser_t));
  native_parser->parser = parser;
  native_parser->settings = settings;
  caml_parser = caml_alloc_custom(&caml_http_parser_struct_ops,
          sizeof(caml_http_parser_t *), 0, 1);
  caml_http_parser_struct_val(caml_parser) = native_parser;

  CAMLreturn(p);
}

static int
on_url_cb(http_parser *parser, const char *at, size_t length)
{
  return 0;
}

static int
on_header_field_cb(http_parser *_, const char *at, size_t len)
{
  return 0;
}

static int
on_header_value_cb(http_parser *_, const char *at, size_t len)
{
  return 0;
}

static int
on_body_cb(http_parser *parser, const char *at, size_t length)
{
  return 0;
}

static int on_message_begin_cb(http_parser* parser)
{
  return 0;
}

static int on_status_complete_cb(http_parser* parser)
{
  return 0;
}

static int on_headers_complete_cb(http_parser* parser)
{
  return 0;
}

static int on_message_complete_cb(http_parser* parser)
{
  return 0;
}

static int
caml_parse_http_parser_settings(value v,
                                http_parser_settings *setting)
{

}

CAMLprim value
caml_http_parser_init(value settings, value type)
{
  CAMLparam1(type);
  CAMLlocal1(caml_parser);

  http_parser *native_parser =
    (http_parser *)malloc(sizeof(http_parser));
  http_parser_settings *native_parser_settings =
    (http_parser_settings *)malloc(sizeof(http_parser_settings));
  int rc = caml_parse_http_parser_settings(settings,
                                           native_parser_settings);
  if (rc == -1) { // TODO: error handling here.
  }
  enum http_parser_type parser_type = caml_http_parser_type_ml2c(type);
  http_parser_init(native_parser, parser_type);
  caml_parser = caml_copy_http_parser(native_parser,
                                      native_parser_settings);

  CAMLreturn(caml_parser);
}

CAMLprim value
caml_http_parser_execute(value parser, value data)
{
  CAMLparam3(parser, setting, data);
  CAMLlocal1(nparsed);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);
  const char *local_data = String_val(data);
  int parsed = http_parser_execute(native_parser->parser,
                      native_parser->settings,
                      local_data,
                      strlen(local_data));
  nparsed = Val_int(parsed);
  CAMLreturn(nparsed);
}

