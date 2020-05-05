#ifndef MMN14_ERRORS_H
#define MMN14_ERRORS_H

/* Labels */
#define ERROR_LABEL_CANNOT_START_WITH_NUM "ERROR: Label name can't start with number on line %d in file %s\n"
#define ERROR_LABEL_NAME_TOO_LONG "ERROR: Label is longer than allowed on line %d in file %s\n"
#define ERROR_LABEL_NAME_NOT_LETTER_OR_NUM "ERROR: Label names must be alphanumeric, violation on line %d in file %s\n"
#define ERROR_LABEL_INVALID_FORMAT "ERROR: Invalid format for label on line %d in file %s\n"
#define ERROR_LABEL_EMPTY "ERROR: Line ended after label declaration on line %d in file %s\n"
#define ERROR_LABEL_ON_EMPTY "ERROR: Line ended on label declaration on line %d in file %s\n"
#define ERROR_LABEL_NAME_IDENTICAL_OP_NAME "ERROR: Label name can't be equal to operation names on line %d in file %s\n"
#define ERROR_LABEL_NAME_IDENTICAL_REG_MAME "ERROR: Label name can't be equal to register names on line %d in file %s\n"
#define ERROR_LABEL_NAME_IDENTICAL_DIR_MAME "ERROR: Label name can't be equal to directive names on line %d in file %s\n"
#define ERROR_LABEL_NO_BRACKET "ERROR: No brackets needed on directive label declaration on line %d in file %s\n"
#define ERROR_LABEL_EXIST "ERROR: Label already exist on line %d in file %s\n"
#define ERROR_ENTRY_LABEL_NOT_FOUND "ERROR: Entry label in line %d not found in file %s \n"
#define ERROR_LABEL_NOT_FOUND "ERROR: Label is not defined in line %d not found in file %s \n"

/* Operations */
#define ERROR_OPERATION_INVALID_FORMAT "ERROR: Unknown operation on line %d in file %s\n"
#define ERROR_OPERATION_EXPECTED_SPACE "ERROR: Expected space after operation on line %d in file %s\n"
#define ERROR_OPERATION_EXPECTED_EOL "ERROR: Expected end of line on line %d in file %s\n"
#define ERROR_OPERATION_EXPECTED_NONE_EOL "ERROR: missing data for operation reading on line %d in file %s\n"
#define ERROR_OPERATION_MISSING "ERROR: missing operand, line ended on line %d in file %s\n"
#define ERROR_OPERATION_MISSING_COMMA "ERROR: missing comma between operands on line %d in file %s\n"
#define ERROR_OPERATION_INVALID_ADDRESS "ERROR: Invalid operand address type for current operation on line %d in file %s\n"


/* general */
#define ERROR_LINE_TOO_LONG "ERROR: Line number %d longer than the max allowed in file %s\n"
#define ERROR_OUT_OF_MEMORY "FATAL: OUT OF MEMORY!\n"
#define ERROR_MISSING_FILE "ERROR: Missing file name as input parameter\n"
#define ERROR_OPEN_FILE "ERROR: Cannot open input file\n"
#define ERROR_WRITE_FILE "ERROR: Cannot open for writing file\n"


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


#endif /*ERRORS_H*/