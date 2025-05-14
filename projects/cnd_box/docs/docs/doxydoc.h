///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////
// Project: C& Programming Language Environment
// Author(s): Anton Yashchenko
// Website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file 
/// Project-wide doxygen documentation settings/pre-definitions.
/// 
/// The C& Programming Language Environment project used '//' to denote regular
/// code comments, and '///' to denote Doxygen documentation.
///////////////////////////////////////////////////////////////////////////////

// Topics, lists of all topics with brief descriptions and hierarchy:

///////////////////////////////////////////////////////////////////////////////
// Top Level Groups
///////////////////////////////////////////////////////////////////////////////

/// @defgroup cppmodule C++ Modules
/// C++ libraries included in the C& Programming Language Environment which are
/// used to describe of the implementation of the language and compiler.

/// @defgroup unittest C++ Unit Tests
/// C++ Unit Tests performed to test this library.

/// @defgroup cand_compiler C& Compiler

/// @defgroup cand_cide C& IDE
/// C& Integrated Development Evironment

///////////////////////////////////////////////////////////////////////////////
// Modules
///////////////////////////////////////////////////////////////////////////////

/// @defgroup cppmodule0_mta Meta Template Programming Archive
/// @ingroup cppmodule

/// @defgroup cppmodule1_minitest Minimal Unit Testing Framework
/// @ingroup cppmodule
/// Single header file unit testing framework for C++20.

/// @defgroup cppmodule2_cppextended C++ Extended Standard Library
/// @ingroup cppmodule

///////////////////////////////////////////////////////////////////////////////
// Unit Tests
///////////////////////////////////////////////////////////////////////////////

/// @defgroup unittest0_cppextended UT C++ Extended Standard Library
/// @ingroup unittest

/// @defgroup unittest1_cand_compiler UT C& Compiler
/// @ingroup unittest

///////////////////////////////////////////////////////////////////////////////
// C& Compiler
///////////////////////////////////////////////////////////////////////////////

/// @defgroup cand_compiler_data Constants & Data Structures
/// @ingroup cand_compiler
/// Constants for the compiler impl including syntax, errors and properties.

/// @defgroup cand_compiler_lexer Lexer
/// @ingroup cand_compiler
/// Performs a lexical analysis. Turns the source into an array of tokens.

/// @defgroup cand_compiler_parser Parser
/// @ingroup cand_compiler
/// Performs a syntatical analysis. Turns an array of tokens into an abstract
/// syntax tree.

/// @defgroup cand_compiler_cppcg C++ Code Generator
/// @ingroup cand_compiler

/// @defgroup cand_compiler_builder Builder
/// @ingroup cand_compiler
/// Handles the build process,and settings, dispatches required work to other 
/// compiler tools and organizes it for a translation unit.

///////////////////////////////////////////////////////////////////////////////
// C& Integrated Development Environment
///////////////////////////////////////////////////////////////////////////////

/// @defgroup cand_cide_cgui Graphical User Interface Bridge
/// @ingroup cand_cide

/// @defgroup cand_cide_common Constants and Utils.
/// @ingroup cand_cide
/// Constants and Utils used in CIDE implementation.

/// @defgroup cand_cide_backend Backend Functionality.
/// @ingroup cand_cide
/// Implementation of the CIDE backend functionality.

/// @defgroup cand_cide_frontend Frontend Interface and Controls.
/// @ingroup cand_cide
/// CIDE graphical user interface and presentation implementation.
