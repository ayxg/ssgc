#pragma once
// This file contains all the macro definitions for ExtCStd
// All other ExtCStd headers should include this file.

// Macro: LAMBDA
// Convention:
// 1. Use lambda macro instead of auto when declaring lambdas
// 2. Lambda variable should be CamelCase with a preceding 'x'.
//    'x' stands for lambda eXpression.
//    ex. LAMBDA xMyLambda = []() {};
#define LAMBDA auto

// Macro: lambda_string
// helper macro to create a lambda that returns a string at compile time.
#define LAMBDA_STRING(x) []() consteval -> auto { return x; }