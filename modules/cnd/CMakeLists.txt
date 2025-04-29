# cnd compiler target config
add_executable(cnd_compiler
  modules/cnd/src/cnd_main.cpp
  modules/cnd/src/cli_main.cpp
  modules/cnd/src/driver.cpp
  #modules/cnd/src/cnd_cst.cpp
  #modules/cnd/src/cnd_lexer.cpp
  #modules/cnd/src/cnd_scanner.cpp
  #modules/cnd/src/cnd_parser.cpp
  #modules/cnd/src/cnd_tk_closure.cpp
  #modules/cnd/src/cnd_tk_cursor.cpp
)

# Add static unit tests.
target_sources(cnd_compiler PRIVATE
  modules/cnd/ut/sut_LexerBasics.cpp
  modules/cnd/ut/sut_LexerLexEscapedCharSequence.cpp
  modules/cnd/ut/sut_LexerLexIdentifier.cpp
  modules/cnd/ut/sut_LexerLexNumber.cpp
  modules/cnd/ut/sut_LexerLexPunctuator.cpp
)

target_include_directories(cnd_compiler PUBLIC 
    modules/cnd/inc
)

target_link_libraries(cnd_compiler
  PRIVATE
    nlohmann_json::nlohmann_json
    # SSG Modules
    ssgmod::mta::mta_interface
    ssgmod::cxxx::cxxx_interface
    ssgmod::wpl::wpl_interface
)