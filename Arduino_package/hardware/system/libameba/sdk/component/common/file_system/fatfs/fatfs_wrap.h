#include "ff.h"
#include "time32.h"

#define PATH_MAX 4096

#if !defined(S_IFDIR)
#define S_IFDIR 0x0040000
#endif
#if !defined(S_IFREG)
#define S_IFREG 0x0100000
#endif
#define DT_DIR S_IFDIR
#define DT_REG S_IFREG

#ifndef __mode_t_defined
  #define __mode_t __MODE_T_TYPE
  #define __MODE_T_TYPE __U32_TYPE
  #define __U32_TYPE unsigned int
  typedef __mode_t mode_t;
  #define __mode_t_defined
#endif

struct dirent {
    /* Always zero */
    long d_ino;

    /* File position within stream */
    long d_off;

    /* Structure size */
    unsigned short d_reclen;

    /* Length of name without \0 */
    size_t d_namlen;

    /* File type */
    int d_type;

    /* File name */
    char d_name[PATH_MAX+1];
};
typedef struct dirent dirent;
typedef int dev_t;
typedef unsigned int ino_t;
//typedef int mode_t;
typedef int nlink_t;
typedef int uid_t;
typedef int gid_t;
typedef signed int off_t;
typedef signed int blksize_t;
typedef signed int blkcnt_t;

struct stat {
    dev_t     st_dev;     /* ID of device containing file */
    ino_t     st_ino;     /* inode number */
    mode_t    st_mode;    /* protection */
    nlink_t   st_nlink;   /* number of hard links */
    uid_t     st_uid;     /* user ID of owner */
    gid_t     st_gid;     /* group ID of owner */
    dev_t     st_rdev;    /* device ID (if special file) */
    off_t     st_size;    /* total size, in bytes */
    blksize_t st_blksize; /* blocksize for file system I/O */
    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
    time_t    st_atime;   /* time of last access */
    time_t    st_mtime;   /* time of last modification */
    time_t    st_ctime;   /* time of last status change */
};

/* access function */
#define	F_OK		0	/* test for existence of file */
#define	X_OK		0x01	/* test for execute or search permission */
#define	W_OK		0x02	/* test for write permission */
#define	R_OK		0x04	/* test for read permission */
  
DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
int scandir(const char *dirp, struct dirent ***namelist,
       int (*filter)(const struct dirent *),
       int (*compar)(const struct dirent **, const struct dirent **));
int rmdir(const char *path);
int mkdir(const char *pathname, mode_t mode);
int access(const char *pathname, int mode);
int stat(const char *path, struct stat *buf);
