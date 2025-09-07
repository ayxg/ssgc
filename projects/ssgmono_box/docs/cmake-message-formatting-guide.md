# CMake Message Formatting Guide
All credit and copyright to the original author : StephenM Nov 10 '22 11:11

[WARNING] DO NOT GO TO LINK, website is filled with ads and popups.
Copied from:
https://www.exchangetuts.com/cmake-multiline-message-with-fatal-error-1640138584343732


You're right. The "simple markup" is either non-indented (unformatted) or indented (formatted). Also, the non-indented text is in paragraphs delimited by newlines. That's why you end up with blank lines in between paragraphs.

Here's a running explanation of the various kinds of messages. Warning types and error types behave the same as far as formatted vs. unformatted text goes. The difference, of course, is what happens to the processing and generation phases of CMake. For readability, you can split strings into multiple double-quoted pieces that will be concatenated.

```
# STATUS
message(STATUS
    "This is a status message. It is prefixed with \"-- \". It goes to stdout. The "
    "lines will wrap according to the width of your terminal.\n"
    "New lines will begin a new line at column 1, but without the \"-- \" prefix, "
    "unless you provide it; they will not create a blank line (i.e., new "
    "paragraph).   Spacing between sentences is unaltered by CMake.\n"
    "-- Here's a new paragraph with an explicit \"-- \" prefix added.")

# no mode given (informational)
message(
    "This is an informational message. It goes to stderr. Each line begins at column "
    "1. The lines will wrap according to the width of your terminal.\n"
    "New lines will begin a new line at column 1; they will not create a blank line "
    "(i.e., new paragraph).   Spacing between sentences is unaltered by CMake (3 spaces "
    "preceded this sentence.).")

# WARNING--unformatted
message(WARNING
    "This is an unformatted warning message.   It goes to stderr. Each line begins "
    "at column 3. The lines will wrap at a particular column (it appears to be "
    "column 77, set within CMake) and wrap back to column 3.\n"
    "New lines will begin a new paragraph, so they will create a blank line. A final "
    "thing about unformatted messages: They will separate sentences with 2 spaces, "
    "even if your string had something  different.")

# WARNING--formatted and unformatted
message(WARNING
    " This is a formatted warning message. It goes to stderr. Formatted lines will"
    " be indented an additional 2 spaces beyond what was provided in the output"
    " string. The lines will wrap according to the width of your terminal.\n"
    " Indented new lines will begin a new line. They will not create a blank line."
    " If you separate sentences with 1 space, that's what you'll get.  If you"
    " separate them with 2 spaces, that's also what you'll get.\n"
    " If you want to control the width of the formatted paragraphs\n"
    " (a good practice), just keep track of the width of each line and place\n"
    " a \"\\n\" at the end of each line.\n \n"
    " And, if you want a blank line between paragraphs, just place \"\\n \\n\"\n"
    " (i.e., 2 newlines separated by a space) at the end of the first paragraph.\n"
    "Non-indented new lines, however, will be treated like unformatted warning "
    "messages, described above. They will begin at and wrap to column 3. They begin "
    "a new paragraph, so they will create a blank line.    There will be 2 spaces "
    "between sentences, regardless of how many you placed after the period (In the "
    "script, there were 4 spaces before this sentence).\n"
    "And, as you'd expect, a second unindented paragraph will be preceded by a "
    "blank line. But why would you mix formatted and unformatted text?")

```

I saved this into Message.cmake and invoked it with cmake -P Message.cmake 2> output.txt. It results in the following stdout:

```
-- This is a status message. It is prefixed with "-- ". It goes to stdout. The lines will wrap according to the width of your terminal.
New lines will begin a new line at column 1, but without the "-- " prefix, unless you provide it; they will not create a blank line (i.e., new paragraph).   Spacing between sentences is unaltered by CMake.
-- Here's a new paragraph with an explicit "-- " prefix added.
The file, output.txt, contains:


This is an informational message. It goes to stderr. Each line begins at column 1. The lines will wrap according to the width of your terminal.
New lines will begin a new line at column 1; they will not create a blank line (i.e., new paragraph).   Spacing between sentences is unaltered by CMake (3 spaces preceded this sentence.).
CMake Warning at MessageScript.cmake:19 (message):
  This is an unformatted warning message.  It goes to stderr.  Each line
  begins at column 3.  The lines will wrap at a particular column (it appears
  to be column 77, set within CMake) and wrap back to column 3.

  New lines will begin a new paragraph, so they will create a blank line.  A
  final thing about unformatted messages: They will separate sentences with 2
  spaces, even if your string had something different.


CMake Warning at MessageScript.cmake:28 (message):
   This is a formatted warning message. It goes to stderr. Formatted lines will be indented an additional 2 spaces beyond what was provided in the output string. The lines will wrap according to the width of your terminal.
   Indented new lines will begin a new line. They will not create a blank line. If you separate sentences with 1 space, that's what you'll get.  If you separate them with 2 spaces, that's also what you'll get.
   If you want to control the width of the formatted paragraphs
   (a good practice), just keep track of the width of each line and place
   a "\n" at the end of each line.

   And, if you want a blank line between paragraphs, just place "\n \n"
   (i.e., 2 newlines separated by a space) at the end of the first paragraph.

  Non-indented new lines, however, will be treated like unformatted warning
  messages, described above.  They will begin at and wrap to column 3.  They
  begin a new paragraph, so they will create a blank line.  There will be 2
  spaces between sentences, regardless of how many you placed after the
  period (In the script, there were 4 spaces before this sentence).

  And, as you'd expect, a second unindented paragraph will be preceded by a
  blank line.  But why would you mix formatted and unformatted text?
```

SUMMARY

INFORMATIONAL MESSAGES (no mode given)

- start at column 1
- wrap in terminal window until newline
- go to stderr
- new paragraphs begin without preceding blank line
- sentence and word spacing preserved

STATUS MESSAGES

- start at column 1, with "-- " prefix on first paragraph
- wrap in terminal window until newline
- go to stdout
- new paragraphs begin without preceding blank line
- sentence and word spacing preserved

UNFORMATTED WARNING AND ERROR MESSAGES (unindented strings)

- start at column 3
- wrap at column 77
- go to stderr
- new paragraphs are preceded by a blank line
- sentences separated by 2 spaces; words by 1 space

FORMATTED WARNING AND ERROR MESSAGES (indented strings)

- start at column 3, plus whatever indentation the string had
- wrap in terminal window until newline
- go to stderr
- new paragraphs begin without preceding blank line
- sentence and word spacing preserved