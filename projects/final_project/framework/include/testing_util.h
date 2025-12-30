/*******************************************************************************
* File: testing_util.h
*
* Purpose:
*   Library to perform unit tests and print testing summaries with prettified
*   output formatting to standard output
*
* Author:
*   Shani Zuniga
*
*******************************************************************************/

#ifndef __TESTING_UTIL_H__
#define __TESTING_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Macros for test formatting */
#define BOLD_FORMAT "\x1b[1;38m"
#define PASS_FORMAT	"\x1b[1;36m"
#define FAIL_FORMAT	"\x1b[1;31m"
#define UNFORMAT	"\x1b[1;0m"

/* printf declaration */
int printf(const char* format, ...);

/* Core macro to perform checks and print error messages */
#define TEST_CHECK(condition, message) \
	Check((condition), (message), __FILE__, __LINE__)

/* Macro to print final test status */
#define TEST_RESULT(status) \
	do { \
		if (0 == (status)) \
		{ \
			printf(PASS_FORMAT "\tPASSED\n" UNFORMAT); \
		} \
	} while (0)

/**
 * @brief Core check function with formatted output
 * 
 * @param condition Test condition to evaluate
 * @param message Error message if test fails
 * @param file Source file where check occurred
 * @param line Line number where check occurred
 * 
 * @return 
 **/
int Check(
	int condition,
	const char* message,
	const char* file,
	int line
);

/**
 * @brief Print testing summary
 *
 * @param status final overall status after all checks
 */
void PrintTestSummary(int status);

#ifdef __cplusplus
}
#endif

#endif /* __TESTING_UTIL_H__ */