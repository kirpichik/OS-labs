//
// Lab work #2, Time in Kalifornia
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  // https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
  setenv("TZ", "America/Tijuana", 1);
  // setenv("TZ", "PST8PDT", 1);

  time_t tm = time(NULL);

  printf("%s", ctime(&tm));
  return 0;
}
