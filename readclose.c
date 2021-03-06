#include <unistd.h>
#include "open.h"
#include "error.h"
#include "readclose.h"

/**
	@file readclose.c
	@author kp
	@source qlibs
	@brief This is the successor of the older 'slurpclose.c' file. The function
   'slurpclose'  is now called  'readclose_append'.  The other function
   'readclose' was introduced here initial.
*/

int readclose_append(int fd,stralloc *sa,unsigned int bufsize)
{
  int r;
  for (;;) {
    if (!stralloc_readyplus(sa,bufsize)) { close(fd); return -1; }
    r = read(fd,sa->s + sa->len,bufsize);
    if (r == -1) if (errno == EINTR) continue;
    if (r <= 0) { close(fd); return r; }
    sa->len += r;
  }
}

int readclose(int fd,stralloc *sa,unsigned int bufsize)
{
  if (!stralloc_copys(sa,"")) { close(fd); return -1; }
  return readclose_append(fd,sa,bufsize);
}

int openreadclose(const char *fn,stralloc *sa,unsigned int bufsize)
{
  int fd;
  fd = open_read((char *) fn);
  if (fd == -1) {
    if (errno == ENOENT) return 0;
    return -1;
  }
  if (readclose(fd,sa,bufsize) == -1) return -1;
  return 1;
}
