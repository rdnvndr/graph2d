#  include <libintl.h>
#  undef _
#  define _(String) dgettext ("graph2d", String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif