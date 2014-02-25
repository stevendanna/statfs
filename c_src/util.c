// This file is part of statfs released under the Apache 2.0 license.
// See the LICENSE file for more information.

#include <errno.h>
#include <sys/statvfs.h>
#include "statfs.h"

ERL_NIF_TERM
make_atom(ErlNifEnv* env, const char* name)
{
    ERL_NIF_TERM ret;
    if(enif_make_existing_atom(env, name, &ret, ERL_NIF_LATIN1)) {
      return ret;
    }
    return enif_make_atom(env, name);
}

ERL_NIF_TERM
make_ok(statfs_st* st, ErlNifEnv* env, ERL_NIF_TERM value)
{
    return enif_make_tuple2(env, st->atom_ok, value);
}

ERL_NIF_TERM
make_error(statfs_st* st, ErlNifEnv* env, const char* error)
{
    return enif_make_tuple2(env, st->atom_error, make_atom(env, error));
}

const char *
resolve_errno(int err)
{
  if (EACCES==err)       return "eacces";
  if (EBADF==err)        return "ebadf";
  if (EFAULT==err)       return "efault";
  if (EINTR==err)        return "eintr";
  if (EIO==err)          return "eio";
  if (ELOOP==err)        return "eloop";
  if (ENAMETOOLONG==err) return "enametoolong";
  if (ENOENT==err)       return "enoent";
  if (ENOMEM==err)       return "enomem";
  if (ENOSYS==err)       return "enosys";
  if (ENOTDIR==err)      return "enotdir";
  if (EOVERFLOW==err)    return "eoverflow";

  return "unknown";
}

ERL_NIF_TERM
make_f_flag(statfs_st* st, ErlNifEnv* env, const struct statvfs* stat)
{
  ERL_NIF_TERM ret = enif_make_list(env, 0);

  if (stat->f_flag & ST_RDONLY) {
    ret = enif_make_list_cell(env, st->atom_rdonly, ret);
  }
  if (stat->f_flag & ST_NOSUID) {
    ret = enif_make_list_cell(env, st->atom_nosuid, ret);
  }

  return ret;
}
