/*
  Erlang port driver for statfs

  Copyright 2014 Jean Parpaillon

  Author: Jean Parpaillon <jean.parpaillon@free.fr>
*/
#include <stdio.h>
#include <errno.h>
#include "statfs.h"

ERL_NIF_TERM
nif_statfs(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  statfs_st* st = (statfs_st*)enif_priv_data(env);
  ERL_NIF_TERM ret;
  char path[MAXBUFLEN] = {0};
  struct statvfs* stat = enif_alloc(sizeof(struct statvfs));

  if (argc != 1) {
    ret = enif_make_badarg(env);
    goto out;
  }

  if (enif_get_string(env, argv[0], path, MAXBUFLEN, ERL_NIF_LATIN1) < 1) {
    ret = enif_make_badarg(env);
    goto out;
  }

  if (statvfs(path, stat)) {
    ret = make_error(st, env, resolve_errno(errno));
    goto out;
  }

  ret = make_statfs(st, env, stat);

 out:
  enif_free(stat);
  return ret;
}

#ifdef __MACH__
ERL_NIF_TERM
nif_mounts(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  statfs_st* st = (statfs_st*)enif_priv_data(env);
  ERL_NIF_TERM ret;
  struct statfs *mntbufp;
  int count = 0;

  if (argc) {
    return(enif_make_badarg(env));
  }

  count = getmntinfo(&mntbufp, 0);
  if (!count) {
    return(make_error(st, env, resolve_errno(errno)));
  }

  ret = enif_make_list(env, 0);
  for(int i = 0; i < count; i++) {
    ret = enif_make_list_cell(env, make_mount(st, env, &mntbufp[i]), ret);
  }

  return(make_ok(st, env, ret));
}
#else
ERL_NIF_TERM
nif_mounts(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  statfs_st* st = (statfs_st*)enif_priv_data(env);
  ERL_NIF_TERM ret;
  FILE* file;
  struct mntent* ent = enif_alloc(sizeof(struct mntent));

  if (argc) {
    ret = enif_make_badarg(env);
    goto out;
  }

  file = setmntent("/proc/mounts", "r");
  if (file == NULL) {
    ret = make_error(st, env, resolve_errno(errno));
    goto out;
  }

  ret = enif_make_list(env, 0);
  while (NULL != (ent = getmntent(file))) {
    ret = enif_make_list_cell(env, make_mount(st, env, ent), ret);
  }
  endmntent(file);

  ret = make_ok(st, env, ret);

 out:
  enif_free(ent);
  return ret;
}
#endif

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
    st->atom_mount = make_atom(env, "mount");

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
  {"statfs", 1, nif_statfs},
  {"mounts", 0, nif_mounts}
};

ERL_NIF_INIT(statfs, funcs, &load, &reload, &upgrade, &unload);
