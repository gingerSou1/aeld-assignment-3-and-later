#include <stdio.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    openlog("writer", LOG_PID, LOG_USER);

    if (argc != 3)
    {
        syslog(LOG_ERR, "Two arguments are required!");
	fprintf(stderr, "ERROR: Two arguments are required!\n");
	closelog();
        return 1;
    }
    const char *writefile = argv[1];
    const char *writestr = argv[2];

    FILE *file = fopen(writefile, "w");

    if (file == NULL)
    {
	syslog(LOG_ERR, "Could not open %s", writefile);
    	fprintf(stderr, "ERROR: Could not open %s\n", writefile);
	closelog();
    	return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    if (fprintf(file, "%s\n", writestr) < 0)
    {
	syslog(LOG_ERR, "Could not write %s", writefile);
    	fprintf(stderr, "ERROR: Could not write to %s\n", writefile);
    	fclose(file);
closelog();
    	return 1;
    }

    if (fclose(file) != 0)
    {
    	syslog(LOG_ERR, "Could not close %s", writefile);
	fprintf(stderr, "ERROR: Could not close %s\n", writefile);
	closelog();
    	return 1;
    }

    closelog();
    return 0;

}
