%% @author Jean Parpaillon <jean.parpaillon@free.fr>
%% @copyright 2014 Jean Parpaillon.
%%% 
%%% This file is provided to you under the Apache License,
%%% Version 2.0 (the "License"); you may not use this file
%%% except in compliance with the License.  You may obtain
%%% a copy of the License at
%%% 
%%%   http://www.apache.org/licenses/LICENSE-2.0
%%% 
%%% Unless required by applicable law or agreed to in writing,
%%% software distributed under the License is distributed on an
%%% "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
%%% KIND, either express or implied.  See the License for the
%%% specific language governing permissions and limitations
%%% under the License.
%%% 
-module(statfs).

-include("statfs.hrl").

%% API
-export([statfs/1, free/1, free_pc/1]).
-export([mounts/0, df/0, df_pretty/0]).

-define(NOT_LOADED, not_loaded(?LINE)).
-on_load(init/0).

%%%
%%% Return free space in bytes
%%%
-spec free(Path :: string()) -> {ok, integer()} | {error, term()}.
free(Path) ->
    case statfs(Path) of
	{ok, #statfs{bsize=Bsize, bfree=Bfree}} ->
	    {ok, Bsize*Bfree};
	{error, Err} ->
	    {error, Err}
    end.

%%%
%%% Return free space as a percentage
%%%
-spec free_pc(Path :: string()) -> {ok, float()} | {error, term()}.
free_pc(Path) ->
    case statfs(Path) of
	{ok, #statfs{frsize=Frsize, blocks=Blocks, bsize=Bsize, bfree=Bfree}} ->
	    Free = (Bsize*Bfree),
	    case (Blocks*Frsize) of
		0 ->
		    {ok, 0.0};
		Total ->
		    {ok, Free / Total}
	    end;
	{error, Err} ->
	    {error, Err}
    end.

%%%
%%% Return list of {mount(), statfs()}
%%%
-spec df() -> [{mount(), statfs()}].
df() ->
    case mounts() of
	{ok, Mounts} ->
	    lists:map(fun(#mount{dir=Dir}=Mount) ->
			      {ok, Stat} = statfs(Dir),
			      {Mount, Stat}
		      end, Mounts);
	{error, Err} ->
	    throw(Err)
    end.

%%%
%%% Return list of pretty mounts figures (sizes in kB)
%%%
df_pretty() ->
    lists:map(fun ({#mount{fsname=Fsname, dir=Dir, type=Type}, 
		    #statfs{bsize=Bsize, blocks=Blocks, bfree=Bfree}}) ->
		      Total = round(Bsize * Blocks / 1024),
		      Used = round(Bsize * (Blocks - Bfree) / 1024),
		      Free = Bsize * Bfree,
		      Pc = case Total of
			       0 -> 0.0;
			       _ -> Used / Total
			   end,
		      {Fsname, Dir, Type, Total, Used, Free, Pc}
	      end, df()).

%%%
%%% statfs NIF
%%%
-spec statfs(Path :: string()) -> {ok, statfs()} | {error, term()}.
statfs(_Path) ->
    {ok, #statfs{}}.

%%%
%%% mounts NIF
%%%
-spec mounts() -> {ok, [mount()]} | {error, term()}.
mounts() ->
    {ok, []}.

%%%===================================================================
%%% Internal functions
%%%===================================================================
init() ->
    PrivDir = case code:priv_dir(?MODULE) of
        {error, _} ->
            EbinDir = filename:dirname(code:which(?MODULE)),
            AppPath = filename:dirname(EbinDir),
            filename:join(AppPath, "priv");
        Path ->
            Path
    end,
    erlang:load_nif(filename:join(PrivDir, "statfs"), 0).

%not_loaded(Line) ->
%    erlang:nif_error({not_loaded, [{module, ?MODULE}, {line, Line}]}).
