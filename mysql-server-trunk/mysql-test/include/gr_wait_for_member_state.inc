# ==== Purpose ====
#
# Wait for this server to reach a given state on the given group
#
#
# ==== Usage ====
#
# --let $group_replication_group_name= UUID
# --let $group_replication_member_state= STATE
# [--let $group_replication_member_id= UUID]
# [--let $wait_timeout= NUMBER]
# [--let $rpl_debug= 1]
# --source include/gr_wait_for_member_state.inc
#
# Parameters:
#   $group_replication_group_name
#     Group name
#
#   $group_replication_member_state
#     The state that the member should have
#
#   $group_replication_member_id
#     The member uuid that should have the given state
#     When not present, the test is done based on the current server uuid
#
#   $wait_timeout
#     See include/wait_condition.inc
#
#   $rpl_debug
#     See include/rpl/init.inc

--let $include_filename= gr_wait_for_member_state.inc
--source include/begin_include_file.inc

if ($group_replication_group_name == '')
{
  --die ERROR IN TEST: the "group_replication_group_name" variable must be set
}

if ($group_replication_member_state == '')
{
  --die ERROR IN TEST: the "group_replication_member_state" variable must be set
}

if (!$rpl_debug)
{
  --disable_query_log
}

--let $_member_id= query_get_value(SELECT @@SERVER_UUID, @@SERVER_UUID, 1)
if ($group_replication_member_id != "")
{
  --let $_member_id= $group_replication_member_id
}

let $wait_condition= SELECT COUNT(*)=1 FROM performance_schema.replication_group_members
  WHERE member_id="$_member_id" AND
  member_state="$group_replication_member_state";
--source include/wait_condition_or_abort.inc

# Due to notify first the change to state ONLINE and after disable super read only
# when the lowest version servers are online we have two cases that need to
# wait for super read only= 0:
#   - when we are starting the primary in the single primary mode
#   - when we are starting a server on multiple primary

if (`SELECT COUNT(*)=1 FROM performance_schema.replication_group_members group_members WHERE group_members.member_id="$_member_id" AND group_members.member_state="ONLINE" AND group_members.member_role="PRIMARY"`)
{
  --source include/gr_find_lowest_member_version.inc
  if (`SELECT COUNT(*)=1 FROM performance_schema.replication_group_members group_members WHERE group_members.member_id="$_member_id" AND group_members.member_state="ONLINE" AND group_members.member_role="PRIMARY" AND group_members.member_version="$group_replication_lowest_member_version_out_var"`)
  {
    --let $wait_condition= SELECT @@GLOBAL.read_only = '0'
    --source include/wait_condition_or_abort.inc
  }
}

--let $include_filename= gr_wait_for_member_state.inc
--source include/end_include_file.inc
