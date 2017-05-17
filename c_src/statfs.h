/*
  Erlang port driver for statfs

  Copyright 2014 Jean Parpaillon

  Author: Jean Parpaillon <jean.parpaillon@free.fr>
*/

#ifndef __STATFS_DRV__
#define __STATFS_DRV__

#include <sys/statvfs.h>

#ifdef __MACH__
#include <sys/param.h>
#include <sys/ucred.h>
#include <sys/mount.h>
#else
#include <mntent.h>
#endif

#include "erl_nif.h"

#define MAXBUFLEN 1024

typedef struct {
  ERL_NIF_TERM    atom_ok;
  ERL_NIF_TERM    atom_error;
  ERL_NIF_TERM    atom_statfs;
  ERL_NIF_TERM    atom_rdonly;
  ERL_NIF_TERM    atom_nosuid;
  ERL_NIF_TERM    atom_mount;
} statfs_st;

ERL_NIF_TERM make_atom(ErlNifEnv* env, const char* name);
ERL_NIF_TERM make_ok(statfs_st* st, ErlNifEnv* env, ERL_NIF_TERM data);
ERL_NIF_TERM make_error(statfs_st* st, ErlNifEnv* env, const char* error);
ERL_NIF_TERM make_statfs(statfs_st* st, ErlNifEnv* env, const struct statvfs* stat);
#ifdef __MACH__
ERL_NIF_TERM make_mount(statfs_st* st, ErlNifEnv* env, const struct statfs* stat);
#else
ERL_NIF_TERM make_mount(statfs_st* st, ErlNifEnv* env, const struct mntent* stat);
#endif

ERL_NIF_TERM make_f_flag(statfs_st* st, ErlNifEnv* env, const struct statvfs* stat);


const char* resolve_errno(int err);

ERL_NIF_TERM nif_statfs(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM nif_mounts(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
#endif
