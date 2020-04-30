/* Labels */
#define ERROR_LABEL_CANNOT_START_WITH_NUM "ERROR: Label name can't start with number on line %d in file %s\n"
#define ERROR_LABEL_NAME_TOO_LONG "ERROR: Label is longer than allowed on line %d in file %s\n"
#define ERROR_LABEL_NAME_NOT_LETTER_OR_NUM "ERROR: Label names must be alphanumeric, violation on line %d in file %s\n"
#define ERROR_LABEL_EMPTY "ERROR: Line ended after label declaration on line %d in file %s\n"


/* general */
#define ERROR_LINE_TOO_LONG "ERROR: Line number %d longer than the max allowed in file %s\n"
#define ERROR_OUT_OF_MEMORY "FATAL: OUT OF MEMORY!\n"

/* Directive */
#define ERROR_UNDEFINED_DIRECTIVE "ERROR: Invalid directive on line  %d in file %s\n"
#define ERROR_EXPECTED_SPACE_DIRECTIVE "ERROR: Expected space after directive on line  %d in file %s\n"

/* Directive Strings*/

#define ERROR_EXPECTED_DOUBLE_BRACKET "ERROR: Expected double bracket for string declaration on line  %d in file %s\n"
#define ERROR_EMPTY_STRING "ERROR: Empty string on line  %d in file %s\n"
#define ERROR_STRING_EOL "ERROR: Line ended without string double bracket encapsulation on line  %d in file %s\n"
#define ERROR_STRING_INVALID "ERROR: Invalid character on string declaration on line  %d in file %s\n"
#define ERROR_STRING_EXPECTED_EOL "ERROR: Line expected to end after string declaration on line  %d in file %s\n"

/* NUMS */
#define ERROR_EXPECTED_NUM_COMMA "ERROR: Comma appear before digits on line  %d in file %s\n"
#define ERROR_MISSING_COMMA "ERROR: Missing comma between numbers on line  %d in file %s\n"
#define ERROR_EXPECTED_NUM_INT "ERROR: Expect to get integer numbers only on line  %d in file %s\n"
#define ERROR_EXPECTED_NUM_EOL "ERROR: Expect to get numbers but line ended  on line %d in file %s\n"
#define ERROR_EXPECTED_NUM "ERROR: Invalid number format on line %d in file %s\n"
#define ERROR_NUM_RANGE "ERROR: Number out of range on line %d in file %s\n"


// /* Labels */
// #define ERROR_EMPTY_LABEL "WARN: Empty label on line %d in file %s\n"
// #define ERROR_USELESS_LABEL "WARN: Useless label '%s', no directive or operation on line %d in file %s\n"
// #define ERROR_LABEL_NOT_ON_START_OF_LINE "ERROR: Labels not at the start of the line on line %d in file %s, label ignored\n"
// #define ERROR_WHITESPACE_AFTER_LABEL "ERROR: Whitespace between label name and colon on line %d in file %s, label ignored\n"
// #define ERROR_DOUBLE_AFTER_LABEL "ERROR: Double colon on line %d in file %s, label ignored\n"
// #define ERROR_LABEL_CANNOT_START_WITH_NUM "ERROR: Label name can't start with number on line %d in file %s\n"
// #define ERROR_LABEL_NAME_IDENTICAL_OP_NAME "ERROR: Label name can't be equal to op names on line %d in file %s\n"
// #define ERROR_LABEL_NAME_IDENTICAL_REG_MAME "ERROR: Label name can't be equal to register names on line %d in file %s\n"
// #define ERROR_LABEL_NAME_IDENTICAL_DIR_MAME "ERROR: Label name can't be equal to directive names on line %d in file %s\n"
// #define ERROR_LABEL_NAME_NOT_LETTER_OR_NUM "ERROR: Label names must be alphanumeric, violation on line %d in file %s\n"
// #define ERROR_LABEL_NAME_NOT_LETTER_OR_NUM "ERROR: Label names must be alphanumeric, violation on line %d in file %s\n"
// #define ERROR_LABEL_DOESNT_EXIST "ERROR: Label \"%s\" does not exist on line %d in file %s\n"
// #define ERROR_LABEL_NAME_TOO_LONG "ERROR: Label is longer than allowed on line %d in file %s\n"
// #define ERROR_NON_ALPHA_LABEL "ERROR: Label should start with a letter on line %d in file %s, label ignored\n"


// /* Parsing */
// #define ERROR_EXPECTED_CHARACTER "ERROR: Expected '%c' but got '%c' (ASCII %d) on line %d in file %s\n"
// #define ERROR_EXPECTED_OPS_NUM "ERROR: Expected command but got digits on line %d in file %s\n"
// #define ERROR_EXPECTED_OPER_EOL "ERROR: Expected operand but got EOL on line %d in file %s\n"
// #define ERROR_EXPECTED_CHARACTER_EOL "ERROR: Expected string but line ended on line %d in file %s\n"
// #define ERROR_EXPECTED_ANY_CHARACTER_EOL "ERROR: Expected a character but line ended on line %d in file %s\n"
// #define ERROR_EXPECTED_ARGUMENT_EOL "ERROR: Line ended unexpectly on line %d in file %s\n"
// #define ERROR_EXPECTED_NUMBER "ERROR: Undefined number on line %d in file %s\n"
// #define ERROR_EXPECTED_INT_NUMBER "ERROR: Real numbers not allowed, on line %d in file %s\n"
// #define ERROR_EXPECTED_NUMBER_EOL "ERROR: Expected a number but line ended on line %d in file %s\n"
// #define ERROR_EXPECTED_WHITESPACE "ERROR: Expected whitespace on line %d in file %s\n"
// #define ERROR_UNEXPECTED_WHITESPACE "ERROR: Unexpected whitespace on line %d in file %s\n"
// #define ERROR_EXPECTED_NUM_WHITESPACE "ERROR: Expected number without whitespace on line %d in file %s\n"
// #define ERROR_EXPECTED_COMMA "ERROR: Expected comma between arguments on line %d in file %s\n"
// #define ERROR_EXPECTED_NUM_COMMA "ERROR: Undefined comma before number on line %d in file %s\n"
// #define ERROR_EXPECTED_EOL "ERROR: Expected end of line on line %d in file %s\n"
// #define ERROR_REGISTER_OUT_OF_BOUNDS "ERROR: Register number %ld is out of bounds (%d..%d) on line %d in file %s\n"
// #define ERROR_LABEL_EXISTS "ERROR: Label \"%s\" already exists on line %d in file %s\n"
// #define ERROR_EMPTY_STRING "ERROR: Empty String found on line %d in file %s\n"

// /* Assembly directives */
// #define ERROR_EMPTY_DIRECTIVE "ERROR: Empty directive on line %d in file %s\n"
// #define ERROR_UNKNOWN_DIRECTIVE "ERROR: Unknown directive on line %d in file %s\n"
// #define ERROR_DATA_OUT_OF_BOUNDS "ERROR: Value %ld is out of bounds (%d..%d) on line %d in file %s\n"
// #define ERROR_INVALID_CHARACTERS "ERROR: Invalid string format on line %d in file %s\n"

// /* General errors */
// #define ERROR_OUT_OF_MEMORY "FATAL: OUT OF MEMORY!\n"

// /* Files */
// #define ERROR_NO_INPUT_FILES "ERROR: Missing input files\n"
// #define ERROR_CANNOT_READ "ERROR: Cannot read the file: %s\n"
// #define ERROR_CANNOT_WRITE "ERROR: Cannot write the file: %s\n"
// #define ERROR_LINE_TOO_LONG "ERROR: Line number %d longer than the max allowed (%d)\n"

// /* OPs */
// #define ERROR_OP_LARGE_NUM "ERROR: The number is bigger then 21 bits on line %d in file %s\n"
// #define ERROR_UNKNOWN_OP "ERROR: Unknown OP name on line %d in file %s\n"
// #define ERROR_MISSING_OPERAND "ERROR: Invalid first operand on line %d in file %s\n"
// #define ERROR_UNSUPPORTED_OPERAND_TYPE "ERROR: The instruction does not support this type of operator on line %d in file %s\n"
