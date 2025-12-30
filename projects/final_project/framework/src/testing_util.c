/*******************************************************************************
* File: testing_util.c
*
* Purpose:
*   implements interface outlined in testing_util.h
*
* Author:
*   Shani Zuniga
*
*******************************************************************************/

/*** Preprocessor Directives **************************************************/

#include <stdio.h> /* printf */

#include "testing_util.h" /* own header */

/* Global variables */
static int g_total_checks = 0;
static int g_failed_checks = 0;


/*** API Functions ************************************************************/

void PrintTestSummary(int status)
{
	printf("\n%s ======= Test Summary =======%s\n", BOLD_FORMAT, UNFORMAT);
	printf("┌──────────────────┬─────────┐\n");
	printf("│ Total checks     │ %7d │\n", g_total_checks);
	printf("│ Failed checks    │ %7d │\n", g_failed_checks);
	printf("└──────────────────┴─────────┘\n");
	if (0 != status)
	{
		printf(FAIL_FORMAT "\nSome tests failed\n" UNFORMAT);
	}
	else
	{
		printf(PASS_FORMAT "\nAll tests completed successfully\n" UNFORMAT);
	}
}

/***************************** CORE TEST FUNCTION *****************************/

/* Core check function implementation */
int Check(int condition, const char* message, const char* file, int line)
{
	++g_total_checks;

	if (!condition)
	{
		printf(FAIL_FORMAT "\n   FAILED: %s\n   └────File: %s; \tLine: %d\n" UNFORMAT,
						message, file, line);
		++g_failed_checks;
		return (1);
	}

	return (0);
}