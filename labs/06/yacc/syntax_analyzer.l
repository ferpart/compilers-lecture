%{
#include <stdio.h>
#include "y.tab.h"

%}

%%
a   {return(A);}
b   {return(B);}
c   {return(C);}
c$  {return(Cfin);}
\n  {return('\n');}
.   { return 0; }
%%
