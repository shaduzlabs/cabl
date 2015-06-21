/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#else
#define syslog(a,b) std::cout<<b<<std::endl;
#endif

#include <k-io.h>
#include "Euklid.h"


using namespace sl;
using namespace sl::kio;

//----------------------------------------------------------------------------------------------------------------------

static void daemonize()
{
#ifdef __linux__
 /* Our process ID and Session ID */
  pid_t pid, sid;

  /* Fork off the parent process */
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  /* If we got a good PID, then we can exit the parent process. */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  /* Change the file mode mask */
  umask(0);
            
  /* Open any logs here */        
  openlog ("k-io_daemon", LOG_PID, LOG_DAEMON);

  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0) {
    /* Log the failure */
    syslog (LOG_ERR, "[k-io] ERROR: Failed to create a new session");
    exit(EXIT_FAILURE);
  }

  /* Change the current working directory */
  if ((chdir("/")) < 0) {
    syslog (LOG_ERR, "[k-io] ERROR: Failed to change the working directory");
    exit(EXIT_FAILURE);
  }

  /* Close out the open file descriptors */
  for (int fd = sysconf(_SC_OPEN_MAX); fd>0; fd--)
  {
    close (fd);
  }
#endif
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
  daemonize();

  Euklid euklid;
  euklid.setMaxConsecutiveErrors(3);
  euklid.run();
  /*
  while(true)
  {
    if(!euklid.connect())
    {
      syslog (LOG_ERR, "[k-io] ERROR: could not connect to device.");
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    else
    {
      syslog (LOG_NOTICE, "[k-io] device connected");
      while(euklid.tick());
      syslog (LOG_NOTICE, "[k-io] device disconnected");
    }
  }
*/
  syslog (LOG_NOTICE, "[k-io] terminated");
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
