#include <stdio.h>

#include "phoneme.h"
#include "spellword.h"

static char *Ascii[] =
	{
" NULL "," START OF HEADING "," START OF TEXT "," END OF TEXT ",
" END OF TRANSMISSION ",
" ENQUIRY "," ACKNOWLEDGEMENT "," BELL "," BACKSPACE "," HORIZONTAL TAB "," LINEFEED ",
" VERTICAL TAB "," FORMFEED "," CARRIAGE RETURN "," SHIFT OUT ",
" SHIFT IN "," DATA LINK ESCAPE "," DEVICE CONTROL ONE "," DEVICE CONTROL TWO ",
" DEVICE CONTROL THREE "," DEVICE CONTROL FOUR "," NEGATIVE ACKNOWLEDGEMENT "," SYNCHRONOUS IDLE ",
" END OF TRANSMISSION BLOCK "," CANCEL "," END OF MEDIUM "," SUBSTITUTE ",
" ESCAPE "," FILE SEPARATOR "," GROUP SEPARATOR "," RECORD SEPARATOR ",
" UNIT SEPARATOR "," SPACE "," EXCLAMATION MARK "," DOUBLE QUOTE ",
" NUMBER SIGN "," DOLLAR SIGN "," PERCENT "," AMPERSAND "," APOSTROPHE ",
" OPEN PAREN "," CLOSE PAREN "," ASTERISK "," PLUS "," COMMA ",
" MINUS "," PERIOD "," SLASH ",

" ZERO "," ONE "," TWO "," THREE "," FOUR ",
" FIVE "," SIX "," SEVEN "," EIGHT "," NINE ",

" COLON "," SEMICOLON "," LESS THAN "," EQUAL SIGN "," GREATER THAN ",
" QUESTION MARK "," AT SIGN ",

" AY "," BEE "," SEE "," DD "," EE "," EFF "," GEE ",
" AITCH "," AYE "," JAY "," KAY "," EL "," EM "," EN "," OH "," PEE ",
" CUE "," AR "," ESS "," TEE "," YOU "," VEE ",
" DOUBLE YOU "," EX "," WY "," ZEE ",

" LEFT BRACKET "," BACKSLASH "," RIGHT BRACKET "," CARET ",
" UNDERSCORE "," BACKTICK ",

" AY "," BEE "," SEE "," DD "," EE "," EFF "," GEE ",
" AITCH "," AYE "," JAY "," KAY "," EL "," EM "," EN "," OH "," PEE ",
" CUE "," AR "," ESS "," TEE "," YOU "," VEE ",
" DOUBLE YOU "," EX "," WY "," ZEE ",

" LEFT BRACE "," PIPE "," RIGHT BRACE "," TILDE "," DEL ",
	};

void say_ascii(int character)
	{
	xlate_word(Ascii[character&0x7F]);
	}

void spell_word(const char *word)
	{
	for (word++ ; word[1] != '\0' ; word++)
		xlate_word(Ascii[(*word)&0x7F]);
	}
