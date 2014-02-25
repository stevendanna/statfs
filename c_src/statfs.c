/*
  Erlang port driver for statfs

  Copyright 2014 Jean Parpaillon

  Author: Jean Parpaillon <jean.parpaillon@free.fr>
*/
#include <sys/statvfs.h>
#include <errno.h>

#include "statfs.h"

ERL_NIF_TERM
nif_statfs(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  statfs_st* st = (statfs_st*)enif_priv_data(env);
  ERL_NIF_TERM ret;
  char path[MAXBUFLEN] = {0};
  struct statvfs* stat = enif_alloc(sizeof(struct statvfs));

  if (enif_get_string(env, argv[0], path, MAXBUFLEN, ERL_NIF_LATIN1) < 1) {
    ret = enif_make_badarg(env);
    goto out;
  }
  
  if (statvfs(path, stat)) {
    //ret = make_error(st, env, resolve_errno(errno));
    ret = make_error(st, env, "error");
    goto out;
  }

  ret = enif_make_tuple(env, 12, st->atom_statfs,
			enif_make_ulong(env, (unsigned long)stat->f_bsize),
			enif_make_ulong(env, (unsigned long)stat->f_frsize),
			enif_make_ulong(env, (unsigned long)stat->f_blocks),
			enif_make_ulong(env, (unsigned long)stat->f_bfree),
			enif_make_ulong(env, (unsigned long)stat->f_bavail),
			enif_make_ulong(env, (unsigned long)stat->f_files),
			enif_make_ulong(env, (unsigned long)stat->f_ffree),
			enif_make_ulong(env, (unsigned long)stat->f_favail),
			enif_make_ulong(env, (unsigned long)stat->f_fsid),
			make_f_flag(st, env, stat),
			enif_make_ulong(env, (unsigned long)stat->f_namemax));

 out:
  enif_free(stat);
  return ret;
}

/*
 * NIF callbacks
 */
static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM info)
{
    statfs_st* st = enif_alloc(sizeof(statfs_st));
    if(st == NULL) {
        return 1;
    }

    st->atom_ok = make_atom(env, "ok");
    st->atom_error = make_atom(env, "error");
    st->atom_statfs = make_atom(env, "statfs");
    st->atom_rdonly = make_atom(env, "rdonly");
    st->atom_nosuid = make_atom(env, "nosuid");

    *priv = (void*) st;

    return 0;
}

static int
reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM info)
{
    return 0;
}

static int
upgrade(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM info)
{
    return load(env, priv, info);
}

static void
unload(ErlNifEnv* env, void* priv)
{
    enif_free(priv);
    return;
}

static ErlNifFunc funcs[] =
{
    {"statfs", 1, nif_statfs}
};

ERL_NIF_INIT(statfs, funcs, &load, &reload, &upgrade, &unload);
