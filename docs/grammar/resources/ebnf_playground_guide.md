## Rules
    - non-terminals must be enclosed in brackets <\nonterminal>
    - Terminals must be surrounded in "quotes".
    - At least one space is necessary between tokens. For example, <\nonterminal>::=... is INVALID. You must have at least one space surrounding '::=' i.e. <\nonterminal> ::= ...
    - Epsilon (which matches nothing) can be sybolized by a capital E. For example, <\very> ::= E | "very" <\very> matches zero or more "very"s in a row, such as veryveryvery (though 'zero or more' can be written more easily using the EBNF '*'). WARNING: Be careful using the epsilon rule. You can easily cause an internal infinite loop that causes the page to become unresponsive. For example, if your BNF in anyway simplifies to E*, compiling will cause an infinite loop and you will have to kill the page and lose any unsaved progress.
    - Comments: Only c++ multi-line style comments are currently supported and must begin and end on its own line: /* interesting comment about something here */

## Syntactic Sugar:
    - Single digit and letter intervals are allowed. For instance, instead of writing: <\e> ::= "1" | "2" | "3", you can write <\e> ::= [\1-3]
    - Basic EBNF is supported. Specifically:
        - "+" can be used to mean "one or more of the previous." For example: <\e> ::= [\a-z]+ can produce any non-zero length sequence of lower case letters.
        - "*" can be used to mean "zero or more of the previous."
        - "?" can be used to mean "zero or one occurances of the previous."
        - "(" ")" can be used to group elements. For example, <\e> ::= ([\1-9] [\a-z])+ allows strings such as: 1a, 4f, 4g3f9d, etc.
        - Inside parentheticals, you may also indicate choice by "|". For example, <\e> ::= ([\1-9] | [\a-z])+ allows strings such as: 2, 3553, 1ffvv2, ggg, etc.
        - Note: EBNF symbols must touch the element(s) they are grouping. valid:( [\1-9]? | [\a-z] )+ invalid: ( [\1-9] || [\a-z] ) +.
        - Check out the short Real Numbers example to see all these EBNF tricks in action.

## Life Hacks:
    - The special symbols backslash (\)\, double quote ("), and new-line (\n) can be a valid terminals when escaped: <\fun> ::= "\\\\" | "\"" | "\n"
    - Basic textual auto-complete is available via ctrl+space.
    - Ctrl-S while editing will compile the grammar & save to your browser history.
    - Each time you hit Ctrl-S OR 'Save BNF as URL' the state is saved in your browser history and you can use the forward and back buttons to navigate saved states.