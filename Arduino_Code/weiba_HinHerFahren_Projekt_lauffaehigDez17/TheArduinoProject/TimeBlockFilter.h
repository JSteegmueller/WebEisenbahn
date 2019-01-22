#ifndef _TimeBlockFilter_h_
#define _TimeBlockFilter_h_

class TimeBlockFilter
{
   unsigned long int sperrzeitMikroSec;
   unsigned long int mikroSecLastEvent;

public:
   TimeBlockFilter(unsigned long int sperrzeitMikroSec);
   bool checkEventAllowed();
};

#endif