// Looking at file attributes

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>				// For ctime()
#include <errno.h>				// Needed to identify any error

char *what_is(mode_t);
void print_err(int);

/*----------------------------------------------------------------------------*/

int main (int argc, char *argv[])
{
struct stat buffer;				// Allocate a structure which is
						// filled in by the system call.

if (argc < 2) printf("Needs an argument.\n");
else
  {
  printf("\nFilename: %s\n", argv[1]);//

  if (lstat(argv[1], &buffer) == -1)		// Use 'lstat' to distinguish
    {						// symbolic links
    printf("That went wrong!\n");
    printf("The error number is %d: ", errno);	// NOTE: O.S. error reporting
    print_err(errno);				// IDs a subset of 'errno's
    }
  else						// Successfully read attributes
    {						// Print a few
    printf("UID: %d\n", buffer.st_uid);
    printf("Size: %lld\n", buffer.st_size);
    printf("Mode (octal): %o\n", buffer.st_mode);// Note *octal* print
    printf("File is a %s.\n", what_is(buffer.st_mode));
    printf("Last modified: %s\n", ctime(&(buffer.st_mtime)));
    }	// Note: 'ctime' is a library call to convert the numeric time int
  }	// a human-friendly string.  Also note it is passed a pointer ('&')
	// to a value within the data structure.
exit(0);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// From the 'man' page, the fields of the Unix file attribute structure:
//
//         struct stat {
//             dev_t     st_dev;     /* ID of device containing file */
//             ino_t     st_ino;     /* inode number */
//             mode_t    st_mode;    /* protection */
//             nlink_t   st_nlink;   /* number of hard links */
//             uid_t     st_uid;     /* user ID of owner */
//             gid_t     st_gid;     /* group ID of owner */
//             dev_t     st_rdev;    /* device ID (if special file) */
//             off_t     st_size;    /* total size, in bytes */
//             blksize_t st_blksize; /* blocksize for file system I/O */
//             blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
//             time_t    st_atime;   /* time of last access */
//             time_t    st_mtime;   /* time of last modification */
//             time_t    st_ctime;   /* time of last status change */
//         };
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* You don't *need* to read below here.                                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// This uses macros from stat.h to decode mode fields.

char *what_is(mode_t mode)
{
if (S_ISREG(mode))  return "regular file";
if (S_ISDIR(mode))  return "directory";
if (S_ISCHR(mode))  return "character device";
if (S_ISBLK(mode))  return "block device";
if (S_ISLNK(mode))  return "symbolic link";
if (S_ISFIFO(mode)) return "FIFO";
if (S_ISSOCK(mode)) return "socket";
else                return "mystery!";
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// This prints messages for a *few* likely error codes.  You could extend it.

void print_err(int error)
{
switch (error)
  {
  case ENOENT: printf("No such file or directory.\n"); break;
  case EACCES: printf("Permission denied.\n");         break;
  case EBADF:  printf("Bad file number.\n");           break;
  case EFAULT: printf("Bad address.\n");               break;
  default:     printf("Something else...\n"); break;	// Here I got bored.
  }
}

/*----------------------------------------------------------------------------*/
