/*
  $NiH: zip_set_name.c,v 1.7 2003/03/16 10:21:41 wiz Exp $

  zip_set_name.c -- rename helper function
  Copyright (C) 1999, 2003 Dieter Baron and Thomas Klausner

  This file is part of libzip, a library to manipulate ZIP files.
  The authors can be contacted at <nih@giga.or.at>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/



#include <stdlib.h>
#include <string.h>
#include "zip.h"
#include "zipint.h"



int
_zip_set_name(struct zip *zf, int idx, const char *name)
{
    if (idx < 0 || idx >= zf->nentry) {
	zip_err = ZERR_INVAL;
	return -1;
    }

    if (name != NULL) {
	if (zf->entry[idx].fn_old == NULL)
	    zf->entry[idx].fn_old = zf->entry[idx].fn;
	else
	    free(zf->entry[idx].fn);
	zf->entry[idx].fn = strdup(name);
	if (zf->entry[idx].fn == NULL) {
	    zf->entry[idx].fn = zf->entry[idx].fn_old;
	    zf->entry[idx].fn_old = NULL;
	    zip_err = ZERR_MEMORY;
	    return -1;
	}
    }

    return 0;
}
