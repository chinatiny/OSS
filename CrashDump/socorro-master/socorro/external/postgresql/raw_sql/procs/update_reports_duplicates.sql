CREATE OR REPLACE FUNCTION update_reports_duplicates 
(
    start_time timestamp with time zone, 
    end_time timestamp with time zone
) 
    RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE 
    new_dups INT;
begin

-- create a temporary table with the new duplicates
-- for the hour
-- this query contains the duplicate-finding algorithm
-- so it will probably change frequently

create temporary table new_reports_duplicates
on commit drop
as
with follower as (
    select 
        uuid
        , install_age
        , uptime
        , client_crash_date
        , date_processed
        -- Find the first report by date and uuid order
        -- that matches this list of crash attributes
        , first_value(uuid) over ( partition by
            product,
            version,
            build,
            signature,
            cpu_name,
            cpu_info,
            os_name,
            os_version,
            address,
            topmost_filenames,
            reason,
            app_notes,
            url
            order by
                client_crash_date
                , uuid
            ) as leader_uuid
    FROM reports
    where date_processed BETWEEN start_time AND end_time
),
leader as (
    -- List of all uuids and timestamp info
    -- for a self-join to reveal duplicates
    select 
        uuid
        , install_age
        , uptime
        , client_crash_date
    FROM reports
    WHERE 
        date_processed BETWEEN start_time AND end_time 
)
select 
    follower.uuid as uuid
	, leader.uuid as duplicate_of
	, follower.date_processed
from
follower
JOIN leader
    ON follower.leader_uuid = leader.uuid
WHERE 
-- Match crashes that are "close enough"
-- by having crash dates and uptimes within
-- 60 seconds
( 
    same_time_fuzzy(
        leader.client_crash_date
        , follower.client_crash_date
        , leader.uptime
        , follower.uptime
    )
    OR 
    follower.uptime < 60
)
AND
    same_time_fuzzy(
        leader.client_crash_date
        , follower.client_crash_date
        , leader.install_age
        , follower.install_age
    )
AND 
    follower.uuid <> leader.uuid;

-- insert a copy of the leaders

insert into new_reports_duplicates
select uuid, uuid, date_processed
from reports
where uuid IN ( select duplicate_of
	from new_reports_duplicates )
	and date_processed BETWEEN start_time AND end_time;

analyze new_reports_duplicates;

select count(*) into new_dups from new_reports_duplicates;

-- insert new duplicates into permanent table

insert into reports_duplicates (uuid, duplicate_of, date_processed )
select new_reports_duplicates.*
from new_reports_duplicates
	left outer join reports_duplicates 
		ON new_reports_duplicates.uuid = reports_duplicates.uuid
		AND reports_duplicates.date_processed > ( start_time - INTERVAL '1 day' )
		AND reports_duplicates.date_processed < ( end_time + INTERVAL '1 day' )
where reports_duplicates.uuid IS NULL;

-- done return number of dups found and exit
DROP TABLE new_reports_duplicates;
RETURN new_dups;
end;$$;
