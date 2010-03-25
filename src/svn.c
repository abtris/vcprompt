#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "svn.h"

#include <ctype.h>

static int
svn_probe(vccontext_t* context)
{
    return isdir(".svn");
}

static result_t*
svn_get_info(vccontext_t* context)
{
    result_t* result = init_result();
    char buf[1024];

    if (!read_first_line(".svn/entries", buf, 1024)) {
        debug("failed to read from .svn/entries: assuming not an svn repo");
        return NULL;
    } else {
	// branch
	*result->branch = system("head -n5 .svn/entries | tail -1 | awk '{ where = match ($1, /(.*)\\/branches\\/(.*)/, arr); if (where==0) { print \"[svn:trunk]\"}  else  { print \"[svn:\"arr[2]\"]\" }  }'");
	// revision
        // *result->revision = system("head -n4 .svn/entries | tail -1 | awk {'print \"[svn:\"$1\"]\" '}");

           }
    return result;
}

vccontext_t* get_svn_context(options_t* options)
{
    return init_context("svn", options, svn_probe, svn_get_info);
}
