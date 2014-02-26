%%% @author Jean Parpaillon <jean.parpaillon@free.fr>
%%% @copyright (C) 2014, Jean Parpaillon
%%% @doc Created from https://github.com/dizz/occi-grammar/blob/master/occi-antlr-grammar/Occi.g
%%%
%%% @end
%%% Created : 25 Feb 2014 by Jean Parpaillon <jean.parpaillon@free.fr>
-type statfs_flag() :: rdonly | nosuid.

-record(statfs, {bsize      = 0  :: integer(),
		 frsize     = 0  :: integer(),
		 blocks     = 0  :: integer(),
		 bfree      = 0  :: integer(),
		 bavail     = 0  :: integer(),
		 files      = 0  :: integer(),
		 ffree      = 0  :: integer(),
		 favail     = 0  :: integer(),
		 fsid       = 0  :: integer(),
		 flag       = [] :: [statfs_flag()],
		 namemax    = 0  :: integer()}).
-type statfs() :: #statfs{}.

-record(mount, {fsname        :: string(),
		dir           :: string(),
		type          :: string(),
		opts          :: string(),
		freq          :: integer(),
		passno        :: integer()}).
-type mount() :: #mount{}.
