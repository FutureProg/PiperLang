// Copyright 2015 The Rust Project Developers. See the COPYRIGHT
// file at the top-level directory of this distribution and at
// http://rust-lang.org/COPYRIGHT.
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ast/ast.h"

extern int yylex();
extern int yyparse();

#define PUSHBACK_LEN 4

static char pushback[PUSHBACK_LEN];
static int verbose;

extern NBlock* root;

void print(const char* format, ...) {
  va_list args;
  va_start(args, format);
  if (verbose) {
    vprintf(format, args);
  }
  va_end(args);
}

// If there is a non-null char at the head of the pushback queue,
// dequeue it and shift the rest of the queue forwards. Otherwise,
// return the token from calling yylex.
int rslex() {
  if (pushback[0] == '\0') {
    return yylex();
  } else {
    char c = pushback[0];
    memmove(pushback, pushback + 1, PUSHBACK_LEN - 1);
    pushback[PUSHBACK_LEN - 1] = '\0';
    return c;
  }
}

// Note: this does nothing if the pushback queue is full. As long as
// there aren't more than PUSHBACK_LEN consecutive calls to push_back
// in an action, this shouldn't be a problem.
void push_back(char c) {
  for (int i = 0; i < PUSHBACK_LEN; ++i) {
    if (pushback[i] == '\0') {
      pushback[i] = c;
      break;
    }
  }
}

extern int yydebug;


int n_nodes;

int const indent_step = 4;

void print_indent(int depth) {
  while (depth) {
    if (depth-- % indent_step == 0) {
      print("|");
    } else {
      print(" ");
    }
  }
}

int main(int argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "-v") == 0) {
    verbose = 1;
  }else if(argc == 2 && strcmp(argv[1], "-d") == 0){
    yydebug = 1;    
  } else {
    verbose = 0;
  }
  int ret = 0;
  memset(pushback, '\0', PUSHBACK_LEN);
  ret = yyparse();
  print("--- PARSE COMPLETE: ret:%d, n_nodes:%d ---\n", ret, n_nodes);
  // if (nodes) {
  //   print_node(nodes, 0);
  // }
  // while (nodes) {
  //   tmp = nodes;
  //   nodes = tmp->next;
  //   if (tmp->own_string) {
  //     free((void*)tmp->name);
  //   }
  //   free(tmp);
  // }
  return ret;
}

void yyerror(char const *s) {
  fprintf(stderr, "%s\n", s);
}
