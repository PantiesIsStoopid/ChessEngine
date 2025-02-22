// misc.c

#include "stdio.h"
#include "defs.h"

#ifdef WIN32
#include "windows.h"
#else
#include "sys/time.h"
#include "sys/select.h"
#include "unistd.h"
#include "string.h"
#endif

int GetTimeMs()
{ // Match the return type in defs.h
#ifdef WIN32
  return GetTickCount();
#else
  struct timeval t;
  gettimeofday(&t, NULL);
  return (t.tv_sec * 1000) + (t.tv_usec / 1000);
#endif
}

// http://home.arcor.de/dreamlike/chess/
int InputWaiting()
{
#ifdef WIN32
  static int init = 0, pipe;
  static HANDLE inh;
  DWORD dw;

  if (!init)
  {
    init = 1;
    inh = GetStdHandle(STD_INPUT_HANDLE);
    pipe = !GetConsoleMode(inh, &dw);
    if (!pipe)
    {
      SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
      FlushConsoleInputBuffer(inh);
    }
  }

  if (pipe)
  {
    if (!PeekNamedPipe(GetStdHandle(STD_INPUT_HANDLE), NULL, 0, NULL, &dw, NULL))
    {
      return 1;
    }
    return dw > 0;
  }
  else
  {
    GetNumberOfConsoleInputEvents(inh, &dw);
    return dw > 1;
  }
#else
  fd_set readfds;
  struct timeval tv;
  FD_ZERO(&readfds);
  FD_SET(fileno(stdin), &readfds);
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  return select(1, &readfds, NULL, NULL, &tv) > 0;
#endif
}

void ReadInput(S_SEARCHINFO *info)
{
  char input[256] = "", *endc;

  if (InputWaiting())
  {
    info->stopped = TRUE;
#ifdef WIN32
    if (!fgets(input, sizeof(input), stdin))
    {
      return;
    }
#else
    int bytes = read(fileno(stdin), input, sizeof(input) - 1);
    if (bytes < 0)
    {
      return;
    }
    input[bytes] = '\0';
#endif

    endc = strchr(input, '\n');
    if (endc)
      *endc = '\0';

    if (strlen(input) > 0)
    {
      if (!strncmp(input, "quit", 4))
      {
        info->quit = TRUE;
      }
    }
  }
}
