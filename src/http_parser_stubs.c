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
#include <stdio.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/signals.h>

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
                      http_parser_settings *native_settings,
                      caml_http_parser_settings_t *caml_settings)
{
  CAMLparam0();
  CAMLlocal1(caml_parser);

  caml_http_parser_t *native_parser = (caml_http_parser_t *)
      malloc(sizeof(caml_http_parser_t));
  native_parser->parser = parser;
  native_parser->native_settings = native_settings;
  native_parser->parser->data = caml_settings;
  caml_parser = caml_alloc_custom(&caml_http_parser_struct_ops,
          sizeof(caml_http_parser_t *), 0, 1);
  caml_http_parser_struct_val(caml_parser) = native_parser;

  CAMLreturn(caml_parser);
}

static int
on_url_cb(http_parser *parser, const char *at, size_t length)
{
  CAMLlocal4(caml_parser, r, data, len);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);
  data = caml_alloc_string(length);
  memcpy(String_val(data), at, length);
  len = Val_int(length);

  r = callback3(settings->on_url, caml_parser, data, len);

  return Int_val(r);
}

static int
on_header_field_cb(http_parser *parser, const char *at, size_t length)
{
  CAMLlocal4(caml_parser, r, data, len);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);
  data = caml_alloc_string(length);
  memcpy(String_val(data), at, length);
  len = Val_int(length);

  r = callback3(settings->on_header_field, caml_parser, data, len);

  return Int_val(r);
}

static int
on_header_value_cb(http_parser *parser, const char *at, size_t length)
{
  CAMLlocal4(caml_parser, r, data, len);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);
  data = caml_alloc_string(length);
  memcpy(String_val(data), at, length);
  len = Val_int(length);

  r = callback3(settings->on_header_value, caml_parser, data, len);

  return Int_val(r);
}

static int
on_body_cb(http_parser *parser, const char *at, size_t length)
{
  CAMLlocal4(caml_parser, r, data, len);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);
  data = caml_alloc_string(length);
  memcpy(String_val(data), at, length);
  len = Val_int(length);

  r = callback3(settings->on_body, caml_parser, data, len);

  return Int_val(r);
}

static int on_message_begin_cb(http_parser* parser)
{
  CAMLlocal2(caml_parser, r);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);

  r = callback(settings->on_message_begin, caml_parser);

  return Int_val(r);
}

static int on_status_complete_cb(http_parser* parser)
{
  CAMLlocal2(caml_parser, r);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);

  r = callback(settings->on_status_complete, caml_parser);

  return Int_val(r);
}

static int on_headers_complete_cb(http_parser* parser)
{
  CAMLlocal2(caml_parser, r);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);

  r = callback(settings->on_headers_complete, caml_parser);

  return Int_val(r);
}

static int on_message_complete_cb(http_parser* parser)
{
  CAMLlocal2(caml_parser, r);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);

  r = callback(settings->on_message_complete, caml_parser);

  return Int_val(r);
}

static int
caml_parse_http_parser_settings(value settings,
                                caml_http_parser_settings_t *native_settings)
{
  CAMLparam1(settings);

  native_settings->on_message_begin    = Field(settings, 0);
  native_settings->on_url              = Field(settings, 1);
  native_settings->on_status_complete  = Field(settings, 2);
  native_settings->on_header_field     = Field(settings, 3);
  native_settings->on_header_value     = Field(settings, 4);
  native_settings->on_headers_complete = Field(settings, 5);
  native_settings->on_body             = Field(settings, 6);
  native_settings->on_message_complete = Field(settings, 7);

  return 0;
}

CAMLprim value
caml_http_parser_init(value settings, value type)
{
  CAMLparam1(type);
  CAMLlocal1(caml_parser);

  http_parser *native_parser =
    (http_parser *)malloc(sizeof(http_parser));

  http_parser_settings *native_settings =
    (http_parser_settings *)malloc(sizeof(http_parser_settings));
  native_settings->on_message_begin = on_message_begin_cb;
  native_settings->on_url = on_url_cb;
  native_settings->on_status_complete = on_status_complete_cb;
  native_settings->on_header_field = on_header_field_cb;
  native_settings->on_header_value = on_header_value_cb;
  native_settings->on_headers_complete = on_headers_complete_cb;
  native_settings->on_body = on_body_cb;
  native_settings->on_message_complete = on_message_complete_cb;

  caml_http_parser_settings_t *caml_settings =
    (caml_http_parser_settings_t *)malloc(sizeof(caml_http_parser_settings_t));
  int rc = caml_parse_http_parser_settings(settings,
                                           caml_settings);
  if (rc == -1) { // TODO: error handling here.
  }
  enum http_parser_type parser_type = caml_http_parser_type_ml2c(type);
  http_parser_init(native_parser, parser_type);
  caml_parser = caml_copy_http_parser(native_parser,
                                      native_settings,
                                      caml_settings);

  CAMLreturn(caml_parser);
}

CAMLprim value
caml_http_parser_execute(value parser, value data)
{
  CAMLparam2(parser, data);
  CAMLlocal1(nparsed);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);
  const char *local_data = String_val(data);
  int parsed = http_parser_execute(native_parser->parser,
                      native_parser->native_settings,
                      local_data,
                      strlen(local_data));
  nparsed = Val_int(parsed);

  CAMLreturn(nparsed);
}

