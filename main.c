#include <stdio.h>
#include "calclib.h"

int main(int argc, char **argv)
{
    int result;
	if (argc < 2) {
		fprintf(stderr, "input expression e.g \"1 + 2\"\n");
		return -1;
	}

	result = calc(argv[1]);
    fprintf(stdout, "%s = %d\n", argv[1], result);
    return 0;
}
