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
make_statfs(statfs_st* st, ErlNifEnv* env, const struct statvfs* stat)
{
  return make_ok(st, env,
		 enif_make_tuple(env, 12, st->atom_statfs,
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
				 enif_make_ulong(env, (unsigned long)stat->f_namemax)));
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


#ifdef __MACH__
ERL_NIF_TERM
make_mount(statfs_st* st, ErlNifEnv* env, const struct statfs* ent)
{
  return enif_make_tuple(env, 7, st->atom_mount,
                         enif_make_string(env, ent->f_mntfromname, ERL_NIF_LATIN1),
                         enif_make_string(env, ent->f_mntonname, ERL_NIF_LATIN1),
                         enif_make_string(env, ent->f_fstypename, ERL_NIF_LATIN1),
                         // We don't support the following fields on OS X
                         enif_make_string(env, "", ERL_NIF_LATIN1),
                         enif_make_int(env, 0),
                         enif_make_int(env, 0));
}
#else
ERL_NIF_TERM
make_mount(statfs_st* st, ErlNifEnv* env, const struct mntent* ent)
{
  return enif_make_tuple(env, 7, st->atom_mount,
			 enif_make_string(env, ent->mnt_fsname, ERL_NIF_LATIN1),
			 enif_make_string(env, ent->mnt_dir, ERL_NIF_LATIN1),
			 enif_make_string(env, ent->mnt_type, ERL_NIF_LATIN1),
			 enif_make_string(env, ent->mnt_opts, ERL_NIF_LATIN1),
			 enif_make_int(env, ent->mnt_freq),
			 enif_make_int(env, ent->mnt_passno));
}
#endif
