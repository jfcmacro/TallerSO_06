#include "utilidad.h"

char* GetLastErrorMessage(DWORD dwError) {

      LPTSTR lptStr = NULL;
      DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

      BOOL fOK = FormatMessage(
			       FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			       FORMAT_MESSAGE_FROM_SYSTEM |
			       FORMAT_MESSAGE_IGNORE_INSERTS,
			       NULL,
			       dwError,
			       systemLocale,
			       (PTSTR) &lpStr,
			       0,
			       NULL);

      if (fOK) {
	

}

