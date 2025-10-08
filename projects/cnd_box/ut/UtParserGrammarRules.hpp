///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_unit_test
/// @brief [UtParserGrammarRules] Validate each parser-stage grammar rule in isolation by calling the associated 
///        parsing method.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{
#pragma once
// !!Keep clang format OFF for this file ,or else expected ast constructors will be unreadable.
// clang-format off
#include "minitest.hpp"
#include "trtools/Parser.hpp"
#include "ParserTestUtils.hpp"

namespace cnd_unit_test::frontend::parser {
// Alias synthesized ast and ast enum to reduce noise for 'expected' ast construction.
using Sast = cnd::Sast;
using enum cnd::eAst;

// Method used for unit testing in this header
using cnd_unit_test::frontend::test_util::TestParsingMethod;
using enum cnd_unit_test::frontend::test_util::eTestParsingMethod;

// The namespace being tested from "trtools/Parser.hpp" header.
using namespace cnd::trtools::parser;

///////////////////////////////////////
/* Primary Statement         */
///////////////////////////////////////
TEST(UtParserGrammarRules, PrimaryStatement) {
  // test a primary expr statement starting with a subexpression.
  TestParsingMethod("(1+2)*a;", ParsePrimaryStatement, 
    Sast{kMul,"(1+2)*a",
      Sast{kAdd,"1+2",
        Sast{kLitInt,"1"},
        Sast{kLitInt,"2"}
      },
      Sast{kIdent,"a"}
    }
    );
}


///////////////////////////////////////
/* Include Statement         */
///////////////////////////////////////
TEST(UtParserGrammarRules, IncludeLocal) {
  TestParsingMethod("include \"foo.cnd\";", ParseIncludeStmt,
    Sast{kIncludeLocalStmt,"",
      Sast{kIQCharSeuquence,"foo.cnd"}
    }
  );
}

TEST(UtParserGrammarRules, IncludeSystem) {
  TestParsingMethod("include <foo.cnd>;", ParseIncludeStmt,
    Sast{kIncludeSystemStmt,"",
      Sast{kIACharSeuquence,"foo.cnd"}
    }
  );
}


/////////////////////////////////////////
///* Pragma Statement         */
/////////////////////////////////////////
TEST(UtParserGrammarRules, PragmaEscaped) {
  TestParsingMethod("pragma \"string_literal\";", ParsePragmaStmt,
    Sast{kPragmaEscapedStmt,"",
      Sast{cnd::eAst::kLitCstr,"string_literal"}
    }
  );
}

TEST(UtParserGrammarRules, PragmaRaw) {
  TestParsingMethod("pragma R\"(raw_string_literal)\";", ParsePragmaStmt,
    Sast{kPragmaEscapedStmt,"",
      Sast{cnd::eAst::kLitCstr,"raw_string_literal"}
    }
  );
}

TEST(UtParserGrammarRules, PragmaFunctional) {
  TestParsingMethod("pragma \"pf_char_sequence\";", ParsePragmaStmt,
    Sast{kPragmaFunctionalStmt,"",
      Sast{kPFCharSequence,"pf_char_sequence"}
    }
  );
}

TEST(UtParserGrammarRules, PragmaName) {
  TestParsingMethod("pragma \"identifier\";", ParsePragmaStmt,
    Sast{kPragmaNamedStmt,"",
      Sast{cnd::eAst::kIdent,"identifier"}
    }
  );
}


///////////////////////////////////////
/* Process Declaration       */
///////////////////////////////////////
TEST(UtParserGrammarRules, ProcessDecl) {
  TestParsingMethod("proc@FooProcess;", ParseProcDecl,
  Sast{kProcessDeclaration,"",
    Sast{kModifiers,""},
    Sast{kIdent,"FooProcess"}
  }
  );
}

TEST(UtParserGrammarRules, ProcessDef) {
  TestParsingMethod("proc@FooProcess:{ };", ParseProcDecl);
}

TEST(UtParserGrammarRules, ProcessDefWithStatements) {
  TestParsingMethod("proc@FooProcess:{const def str@Foo: 42;using @MyInteger: int;};", ParseProcDecl,
    Sast{kProcessDeclaration,"",
      Sast{kModifiers,"",
        Sast{kKwConst,""},
        Sast{kKwStatic,""}
      },
      Sast{kIdent,"FooProcess"},
      Sast{kProcessDefinition,"",
        Sast{kVariableDeclaration,"",
          Sast{kModifiers,"",
            Sast{kKwConst,""}
          },
          Sast{kKwStr,""},
          Sast{kIdent,"Foo"},
          Sast{kVariableDefinition,"",
            Sast{kLitInt,"42"}
          }
        },
        Sast{kTypeAlias,"",
          Sast{kIdent,"MyInteger"},
          Sast{kKwInt,""}
        }
      }
    }
  );
}


///////////////////////////////////////
/* Library Declaration       */
///////////////////////////////////////
TEST(UtParserGrammarRules, LibDecl) {
  TestParsingMethod("lib @FooLibrary;", ParseLibDecl,
  Sast{kProcessDeclaration,"",
    Sast{kModifiers,""},
    Sast{kIdent,"FooLibrary"}
  }
  );
}

TEST(UtParserGrammarRules, LibDef) {
  TestParsingMethod("lib@FooProcess:{ };", ParseLibDecl);
}

TEST(UtParserGrammarRules, LibWithMod) {
  TestParsingMethod("const static lib@MathLib;", ParseLibDecl,
    Sast{kLibraryDeclaration,"",
      Sast{kModifiers,"",
        Sast{kKwConst,""},
        Sast{kKwStatic,""}
      },
      Sast{kIdent,"MathLib"}
    }
  );
}

TEST(UtParserGrammarRules, LibWithModAndDefinition) {
  TestParsingMethod("const static lib@MathLib:{const def str@Foo: 42;using @MyInteger: int;};", ParseLibDecl,
    Sast{kLibraryDeclaration,"",
      Sast{kModifiers,"",
        Sast{kKwConst,""},
        Sast{kKwStatic,""}
      },
      Sast{kIdent,"MathLib"},
      Sast{kLibraryDefinition,"",
        Sast{kVariableDeclaration,"",
          Sast{kModifiers,"",
            Sast{kKwConst,""}
          },
          Sast{kKwStr,""},
          Sast{kIdent,"Foo"},
          Sast{kVariableDefinition,"",
            Sast{kLitInt,"42"}
          }
        },
        Sast{kTypeAlias,"",
          Sast{kIdent,"MyInteger"},
          Sast{kKwInt,""}
        }
      }
    }
  );
}


///////////////////////////////////////
/* Type Alias                */
///////////////////////////////////////
TEST(UtParserGrammarRules, TypeAlias) {
  TestParsingMethod("using @MyInteger: int;", ParseUsingDecl,
    Sast{kTypeAlias,"",
      Sast{kIdent,"MyInteger"},
      Sast{kKwInt,""}
    }
  );
}

TEST(UtParserGrammarRules, LibraryNamespaceInclusion) {
 TestParsingMethod("using lib my_math_lib;", ParseUsingDecl,
    Sast{kLibraryNamespaceInclusion,"",
      Sast{kIdent,"my_math_lib"}
    }    
  );
}

TEST(UtParserGrammarRules, NamespaceInclusion) {
  TestParsingMethod("using namespace my_ns;", ParseUsingDecl);
}

TEST(UtParserGrammarRules, ObjectInclusion) {
  TestParsingMethod("using my_ns::Foo;", ParseUsingDecl);
}

TEST(UtParserGrammarRules, TypeInclusion) {
  TestParsingMethod("using@MyFooType: my_ns::Foo;", ParseUsingDecl);
}

TEST(UtParserGrammarRules, ObjectInclusionFromLibrary) {
  TestParsingMethod("using lib my_math_lib::add;", ParseUsingDecl);
}

TEST(UtParserGrammarRules, TypeInclusionFromLibrary) {
  TestParsingMethod("using @MyAddMethodImpl: lib my_math_lib::add;", ParseUsingDecl);
}

TEST(UtParserGrammarRules, TypeImportDeclaration) {
  TestParsingMethod("import foo;", ParseImportDecl);
}


///////////////////////////////////////
/* Variable Declaration      */
///////////////////////////////////////
TEST(UtParserGrammarRules, VariableDeclarationNoTypeNoAssignNoMod) {
  TestParsingMethod("def@Foo;", ParseVariableDecl,
    Sast{kVariableDeclaration,"",
      Sast{kModifiers,""},
      Sast{kKwAny,""},
      Sast{kIdent,"Foo"}
    }
  );
}

TEST(UtParserGrammarRules, VariableDeclarationNoTypeNoAssign) {
  TestParsingMethod("const def@Foo;", parser::ParseVariableDecl,
    Sast{kVariableDeclaration,"",
      Sast{kModifiers,"",
        Sast{kKwConst,""}
      },
      Sast{kKwAny,""},
      Sast{kIdent,"Foo"}
    }
  );
}

TEST(UtParserGrammarRules, VariableDeclarationNoAssign) {
  TestParsingMethod("const def str@Foo;", parser::ParseVariableDecl,
    Sast{kVariableDeclaration,"",
      Sast{kModifiers,"",
        Sast{kKwConst,""}
      },
      Sast{kKwStr,""},
      Sast{kIdent,"Foo"}
    }
  );
}

TEST(UtParserGrammarRules, VariableDefinition) {
  TestParsingMethod("const def str@Foo: 42;", ParseVariableDecl,
    Sast{kVariableDeclaration,"",
      Sast{kModifiers,"",
        Sast{kKwConst,""}
      },
      Sast{kKwStr,""},
      Sast{kIdent,"Foo"},
      Sast{kVariableDefinition,"",
        Sast{kLitInt,"42"}
      }
    }
  );
}


///////////////////////////////////////
/* Function Declaration */
///////////////////////////////////////
TEST(UtParserGrammarRules, MethodDeclImplicitVoidArgNoRet) {
  TestParsingMethod("fn@add;", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,""}
    }    
  );
}

TEST(UtParserGrammarRules, MethodDeclImplicitVoidArgNoRet2) {
  TestParsingMethod("fn@add();", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kMethodVoid,""}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kMethodVoid,""}
        }
      }
    }    
  );
}

TEST(UtParserGrammarRules, MethodDeclImplicitVoidArgAnyRet) {
  TestParsingMethod("fn@add>;", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kMethodVoid,""}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kKwAny,""}
        }
      }
    }
  );
}

TEST(UtParserGrammarRules, MethodDeclImplicitVoidArgAnyRet2) {
  TestParsingMethod("fn@add()>;", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kMethodVoid,""}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kKwAny,""}
        }
      }
    }    
  );
}

TEST(UtParserGrammarRules, MethodDeclArgNoRet) {
  TestParsingMethod("fn@add(a,b);", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kModifiers,"",
              Sast{kNONE,""}
            },
            Sast{kKwAny,""},
            Sast{kIdent,"a"}
          },
          Sast{kMethodParameter,"",
            Sast{kModifiers,"",
              Sast{kNONE,""}
            },
            Sast{kKwAny,""},
            Sast{kIdent,"b"}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kMethodVoid,""}
        }
      }
    }    
  );
}

TEST(UtParserGrammarRules, MethodDeclArgAnyRet) {
  TestParsingMethod("fn@add(a,b)>;", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kModifiers,"",
              Sast{kNONE,""}
            },
            Sast{kKwAny,""},
            Sast{kIdent,"a"}
          },
          Sast{kMethodParameter,"",
            Sast{kModifiers,"",
              Sast{kNONE,""}
            },
            Sast{kKwAny,""},
            Sast{kIdent,"b"}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kKwAny,""}
        }
      }
    }  
  );
}

TEST(UtParserGrammarRules, MethodDeclArgIdentifiedAnyRet) {
  TestParsingMethod("fn@add(@a,@b)>;", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kModifiers,""},
            Sast{kKwAny,""},
            Sast{kIdent,"a"}
          },
          Sast{kMethodParameter,"",
            Sast{kModifiers,""},
            Sast{kKwAny,""},
            Sast{kIdent,"b"}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kKwAny,""}
        }
      }
    }
  );
}

TEST(UtParserGrammarRules, MethodDeclArgsTypedRet) {
  TestParsingMethod("fn@add(@a,@b)>int;", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kModifiers,""},
            Sast{kKwAny,""},
            Sast{kIdent,"a"}
          },
          Sast{kMethodParameter,"",
            Sast{kModifiers,""},
            Sast{kKwAny,""},
            Sast{kIdent,"b"}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kKwInt,""}
        }
      }
    }
  );
}

TEST(UtParserGrammarRules, MethodDeclTypedArgsTypedRet) {
  TestParsingMethod("fn@add(int @a,int @b)>int;", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kModifiers,""},
            Sast{kKwInt,""},
            Sast{kIdent,"a"}
          },
          Sast{kMethodParameter,"",
            Sast{kModifiers,""},
            Sast{kKwInt,""},
            Sast{kIdent,"b"}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kKwInt,""}
        }
      }
    }    
  );
}

TEST(UtParserGrammarRules, MethodDeclTypedArgsTypedRetWithModifiers) {
  TestParsingMethod("fn@add(const int @a,const int @b)>const int;", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kModifiers,"",
              Sast{kKwConst,""}
            },
            Sast{kKwInt,""},
            Sast{kIdent,"a"}
          },
          Sast{kMethodParameter,"",
            Sast{kModifiers,"",
              Sast{kKwConst,""}
            },
            Sast{kKwInt,""},
            Sast{kIdent,"b"}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kModifiers,"",
            Sast{kKwConst,""}
          },
          Sast{kKwInt,""}
        }
      }
    }    
  );
}

TEST(UtParserGrammarRules, MethodDefinition) {
  TestParsingMethod("fn@add(const int @a,const int @b)>const int:{a+b;};", ParseMethodDecl,
    Sast{kMethodDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"add"},
      Sast{kMethodSignature,"",
        Sast{kMethodParameterList,"",
          Sast{kMethodParameter,"",
            Sast{kModifiers,"",
              Sast{kKwConst,""}
            },
            Sast{kKwInt,""},
            Sast{kIdent,"a"}
          },
          Sast{kMethodParameter,"",
            Sast{kModifiers,"",
              Sast{kKwConst,""}
            },
            Sast{kKwInt,""},
            Sast{kIdent,"b"}
          }
        },
        Sast{kMethodReturnType,"",
          Sast{kModifiers,"",
            Sast{kKwConst,""}
          },
          Sast{kKwInt,""}
        }
      },
      Sast{kMethodDefinition,"",
        Sast{kAdd,"",
          Sast{kIdent,"a"},
          Sast{kIdent,"b"}
        }
      }
    }
  );
}


///////////////////////////////////////
/* Main Declaration */
///////////////////////////////////////
TEST(UtParserGrammarRules, MainDefinition) {
  TestParsingMethod("main(a,b):{a+b;};", ParseMainDecl);
}


///////////////////////////////////////
/* Class Declaration */
///////////////////////////////////////
TEST(UtParserGrammarRules, ClassDecl) {
  TestParsingMethod(
    "class@Husky;", 
    ParseClassDecl,
    Sast{kClassDeclaration,"",
      Sast{kModifiers,""},
      Sast{kIdent,"Husky"}
    },
    test_util::eTestParsingMethod::kCompareSignificantOnly
  );
}

TEST(UtParserGrammarRules, ClassDeclWithMod) {
  TestParsingMethod(
    "const static class@Husky;", 
    ParseClassDecl,
    Sast{kClassDeclaration,"",
      Sast{kModifiers,"",
        Sast{kKwConst,""},
        Sast{kKwStatic,""}
      },
      Sast{kIdent,"Husky"}
    },
    test_util::eTestParsingMethod::kCompareSignificantOnly
  );
}

TEST(UtParserGrammarRules, ClassWithModAndDefinition) {
  TestParsingMethod(
    R"(
    const static class@Husky:{
      const def str@Foo: 42;
      using @MyInteger: int;
    })", 
    ParseClassDecl,
    Sast{kClassDeclaration,"",
      Sast{kModifiers,"",
        Sast{kKwConst,""},
        Sast{kKwStatic,""}
      },
      Sast{kIdent,"Husky"},
      Sast{kClassDefinition,"",
        Sast{kVariableDeclaration,"",
          Sast{kModifiers,"",
            Sast{kKwConst,""}
          },
          Sast{kKwStr,""},
          Sast{kIdent,"Foo"},
          Sast{kVariableDefinition,"",
            Sast{kLitInt,"42"}
          }
        },
        Sast{kTypeAlias,"",
          Sast{kIdent,"MyInteger"},
          Sast{kKwInt,""}
        }
      }
    },
    test_util::eTestParsingMethod::kCompareSignificantOnly
  );
}


///////////////////////////////////////
/* Namespace Declaration */
///////////////////////////////////////
TEST(UtParserGrammarRules, NamespaceDecl) {
  TestParsingMethod(
    "class@Husky;", 
    ParseClassDecl,
    Sast{kNamespaceDecl,"",
      Sast{kIdent,"Husky"}
    },
    test_util::eTestParsingMethod::kCompareSignificantOnly
  );
}


///////////////////////////////////////
/* Enum Declaration */
///////////////////////////////////////
TEST(UtParserGrammarRules, BasicEnumDefinition) {
  TestParsingMethod("enum @eResult:{@Good;@Bad;}", ParseEnumDecl,
  Sast{kEnumDeclaration,"",
    Sast{kModifiers,""},
    Sast{kIdent,"eResult"},
    Sast{kEnumDefinition,"",
      Sast{kEnumBlock,"",
        Sast{kEnumEntry,"Good"},
        Sast{kEnumEntry,"Bad"}
      }
    }
  },test_util::eTestParsingMethod::kCompareSignificantOnly);
}

TEST(UtParserGrammarRules, EnumDefinitionWithValues) {
  TestParsingMethod("enum @eResult:{@Good:0;@Bad:1;}", ParseEnumDecl);
}

TEST(UtParserGrammarRules, EnumDefinitionWithPositionalAssociatedValue) {
  TestParsingMethod("enum @eResult:str:{@Good:1:\"Good\";@Bad:\"Bad\";}", ParseEnumDecl);
}

TEST(UtParserGrammarRules, EnumDefinitionWithNamedAssociatedValue) {
  TestParsingMethod("enum @eResult:str @EnumStr:{@Good:1:\"Good\";@Bad:2:\"Bad\";}", ParseEnumDecl);
}

TEST(UtParserGrammarRules, EnumDefinitionWithTaggedEntries) {
  TestParsingMethod("enum @eResult:str @EnumStr:{@Good:1:\"Good\";tag@ErrorState:@Bad:1:\"Bad\";}", ParseEnumDecl);
}


///////////////////////////////////////
/* Return Statement */
///////////////////////////////////////
TEST(UtParserGrammarRules, ReturnStatement) {
  TestParsingMethod("return a + b;", ParseReturnStmt);
}


///////////////////////////////////////
/* 'if' Statement */
///////////////////////////////////////
TEST(UtParserGrammarRules, IfStatement) {
  TestParsingMethod("if(a){ a + b; };", ParseIfDecl);
}

TEST(UtParserGrammarRules, IfElseStatement) {
  TestParsingMethod("if(a){ a + b; }else{a;};", ParseIfDecl);
}

TEST(UtParserGrammarRules, IfElifStatement) {
  TestParsingMethod("if(a){ a + b; }elif(b){a;};", ParseIfDecl);
}

TEST(UtParserGrammarRules, IfElifElseStatement) {
  TestParsingMethod("if(a){ a + b; }elif(b){a;}else{b;};", ParseIfDecl);
}


///////////////////////////////////////
/* 'switch' Statement */
///////////////////////////////////////
TEST(UtParserGrammarRules, SwitchStatement) {
  TestParsingMethod("switch(foo):{case 1:return 1; case 2: return 2;}", ParseSwitchStmt);
}

TEST(UtParserGrammarRules, SwitchStatementWithDefault) {
  TestParsingMethod("switch(foo):{case 1:return 1; default: return 2;}", ParseSwitchStmt);
}


///////////////////////////////////////
/* 'while' Statement */
///////////////////////////////////////
TEST(UtParserGrammarRules, WhileStatement) {
  TestParsingMethod("while(a){ a + b; };", ParseWhileDecl);
}


///////////////////////////////////////
/* 'for' Statement */
///////////////////////////////////////
TEST(UtParserGrammarRules, ForStatement) {
  TestParsingMethod("for(def@a:0;a!=end;a++){ a + b; };", ParseForDecl);
}


///////////////////////////////////////
/* Uncategorized. */
///////////////////////////////////////
TEST(UtParserGrammarRules, ParseProgramWithDeclarations) {
  TestParsingMethod(
      "import foo;"
      "const static lib@MathLib;"
      "using @MyAddMethodImpl: lib MathLib::add;"
      "const def str@Foo: 42;"
      "fn@add(const int @a,const int @b)>const int;"
      "const static class @Husky;",
      ParseSyntax);
}

TEST(UtParserGrammarRules, AnimalsExampleProgram) {
  TestParsingMethod(
R"(
proc AnimalSounds:{
  class @Horse: {
    fn @makeSound():{return "Neigh!";}
  }
 
  class @Cow: {
    fn @makeSound():{return "Moo!";}
  }
 
  class @Wolf: {
    fn @makeSound():{return "Oooo!";}
  }
 
  class @Cricket: {
    fn @makeSound():{return "Chirp!";}
  } 

  def @farm_animals : list(Horse(), Cow());
  def @all_animals : farm_animals + list(Wolf(), Cricket());

  fn @makeAnimalSounds(list @animal_list) : {
    def str @sounds;
    for(def @idx:0; idx < animal_list.Size(); idx++){
      sounds += animal_list[idx].makeSound();
    }
    return sounds;
  }

  main : makeAnimalSounds(all_animals);
}
)",
      ParseSyntax);
}

// Pragmatic statements appears at program top level, or in a library.
// This tests that the ParsePragmaticStmt can handle all statement types (none were missed).
TEST(UtParserGrammarRules, PragmaticDeclarations) {
  TestParsingMethod("const def str@Foo: 42;", ParsePragmaticStmt);
  TestParsingMethod("using @MyInteger: int;", ParsePragmaticStmt);
  TestParsingMethod("using lib my_math_lib;", ParsePragmaticStmt);
  TestParsingMethod("using namespace my_ns;", ParsePragmaticStmt);
  TestParsingMethod("using my_ns::Foo;", ParsePragmaticStmt);
  TestParsingMethod("using@MyFooType: my_ns::Foo;", ParsePragmaticStmt);
  TestParsingMethod("using lib my_math_lib::add;", ParsePragmaticStmt);
  TestParsingMethod("using @MyAddMethodImpl: lib my_math_lib::add;", ParsePragmaticStmt);
  TestParsingMethod("const static lib@MathLib;", ParsePragmaticStmt);
  TestParsingMethod("import foo;", ParsePragmaticStmt);
  TestParsingMethod("fn@add;", ParsePragmaticStmt);
  TestParsingMethod("fn@add();", ParsePragmaticStmt);
  TestParsingMethod("fn@add>;", ParsePragmaticStmt);
  TestParsingMethod("fn@add()>;", ParsePragmaticStmt);
  TestParsingMethod("fn@add(a,b)>;", ParsePragmaticStmt);
  TestParsingMethod("fn@add(@a,@b)>int;", ParsePragmaticStmt);
  TestParsingMethod("fn@add(int @a,int @b)>int;", ParsePragmaticStmt);
  TestParsingMethod("fn@add(const int @a,const int @b)>const int;", ParsePragmaticStmt);
  TestParsingMethod("class@Husky;", ParsePragmaticStmt);
  TestParsingMethod("const static class@Husky;", ParsePragmaticStmt);
  TestParsingMethod("const static lib@MathLib;", ParsePragmaticStmt);
}

// clang-format on
}  // namespace cnd_unit_test::frontend::parser
/// @} // end of cnd_unit_test

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// This program is free software : you can redistribute it and / or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////