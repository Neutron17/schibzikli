#include "arg.h"
#include <getopt.h>

#include "lt.h"
#include "global.h"
#include "log.h"
#include "exitCodes.h"

struct Arg getArgs(int argc, char *argv[]) {
	struct Arg ret = { 0 };
	int choice;
	while (true) {
		static struct option long_options[] = {
			/* Use flags like so:
			{"verbose",	no_argument,	&verbose_flag, 'V'}*/
			/* Argument styles: no_argument, required_argument, Option_argument */
			{"version", no_argument,	0,	'v'},
			{"help",	no_argument,	0,	'h'},

			{0,0,0,0}
		};

		int option_index = 0;

		/* Argument parameters:
			no_argument: " "
			required_argument: ":"
			Option_argument: "::" */

		choice = getopt_long(argc, argv, "vh",
					long_options, &option_index);

		if (choice == -1)
			break;

		switch(choice) {
			case 'v':

				break;

			case 'h':

				break;

			case '?':
				/* getopt_long will have already printed an error */
				break;

			default:
				/* Not sure how to get here... */
				cleanUp(E_UNKNOWN);
		}
	}

	/* Deal with non-option arguments here */
	while ( optind < argc ) {
		optind++;
	}
	return ret;
}

void parseArgs(struct Arg arg) {
	arg.isDebug = arg.isDebug;
	// TODO
}

