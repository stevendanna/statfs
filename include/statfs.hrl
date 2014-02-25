%%% @author Jean Parpaillon <jean.parpaillon@free.fr>
%%% @copyright (C) 2014, Jean Parpaillon
%%% @doc Created from https://github.com/dizz/occi-grammar/blob/master/occi-antlr-grammar/Occi.g
%%%
%%% @end
%%% Created : 25 Feb 2014 by Jean Parpaillon <jean.parpaillon@free.fr>
-type statfs_flag() :: rdonly | nosuid.

-record(statfs, {bsize        :: integer(),
		 frsize       :: integer(),
		 blocks       :: integer(),
		 bfree        :: integer(),
		 bavail       :: integer(),
		 files        :: integer(),
		 ffree        :: integer(),
		 favail       :: integer(),
		 fsid         :: integer(),
		 flag         :: [statfs_flag()],
		 namemax      :: integer()}).
-type statfs() :: #statfs{}.
