#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h> //For finding size of file

#define FILEPATH "data.dat"
#define NUMINTS  (10)

int main()
{
    int i;
    int fd;
    int *map;  /* mmapped array of int's */

    //Find the size
    struct stat filestatus;
    stat(FILEPATH, &filestatus);
    unsigned long size = filestatus.st_size;
    int array_length = size/sizeof(int) - 1; //The end of file has \0, which we don't want to count.


    fd = open(FILEPATH, O_RDONLY);
    if (fd == -1) {
	perror("Error opening file for reading");
	exit(EXIT_FAILURE);
    }

    map = (int*)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
	close(fd);
	perror("Error mmapping the file");
	exit(EXIT_FAILURE);
    }
    
    /* Read the file int-by-int from the mmap
     */
    for (i = 0; i <=array_length; ++i) {
	printf("%d: %d\n", i, map[i]);
    }

    if (munmap(map, size) == -1) {
	perror("Error un-mmapping the file");
    }
    close(fd);
    return 0;
}