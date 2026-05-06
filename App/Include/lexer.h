#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "vector.h"
#include "tokenTypes.h"

bool isCharSkippable(const char c);

bool isInt(char c);

bool isAlpha(char c);

Vector tokenize(const char *sourceCode);