#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "git.h"


static int
git_probe(vccontext_t* context)
{
    return isdir(".git");
}

static result_t*
git_get_info(vccontext_t* context)
{
    result_t* result = init_result();
    char buf[1024];

    if (!read_first_line(".git/HEAD", buf, 1024)) {
        debug("unable to read .git/HEAD: assuming not a git repo");
        return NULL;
    }
    else {
        char* prefix = "ref: refs/heads/";
        int prefixlen = strlen(prefix);

        if (strncmp(prefix, buf, prefixlen) == 0) {
            /* yep, we're on a known branch */
            debug("read a head ref from .git/HEAD: '%s'", buf);
            result->branch = strdup(buf+prefixlen); /* XXX mem leak! */	// detection git-svn
	    if (!read_first_line(".git/config", buf, 1024)) {
   	      debug(".git/config doesn't exists");	
	     } else {
  	        if (system("grep svn-remote .git/config >/dev/null")==0) {
		strcat(result->branch,":git-svn");
	     }
	    }


        }
        else {
            debug(".git/HEAD doesn't look like a head ref: unknown branch");
            result->branch = "(unknown)";
	}

    }

    return result;
}

vccontext_t* get_git_context(options_t* options)
{
    return init_context("git", options, git_probe, git_get_info);
}
