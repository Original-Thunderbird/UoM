-- [header]
--
-- COMP23111 Fundamentals of Databases
-- Exercise 05
-- by Haorui Chen, ID 10407315, login name z26937hc

-- [opening]
SET ECHO ON -- causes the SQL statements themselves to be spooled
SPOOL EX05-10407315.log
-- sends everything to EX05-10407315.log
-- here you can set the SQL*Plus parameters, such as column width,
-- that will allow the script to produce readable answers in the spool
-- file

-- [body]
start /opt/info/courses/COMP23111/create-Orinoco-tables-complete.sql
start /opt/info/courses/COMP23111/populate-Orinoco-tables-complete.sql

-- 1
select * from ContractInfo;

create or replace trigger temp
  before insert or update on ContractInfo
  for each row
  begin
    set new.duration=1;
  end;
/

insert into ContractInfo (hasContract, date_from, date_to) values('Scandal', '30-oct-2012', '19-jan-2016');
select * from itemType;

update ContractInfo set hasContract='JZTest' where hasContract='JZ';
select * from itemType;

-- 2
create or replace procedure protemp (name VARCHAR2, date_op DATE, date_ed DATE, dur INTEGER) as
  cursor con_cur is
    select date_from, date_to from ContractInfo;
  begin
    if date_op < date_ed then
      dbms_output.put_line('date_op < date_ed');
      return;
    end if;
    for row in con_cur loop
      if row.date_from<date_op and row.date_to>date_op then
        dbms_output.put_line('row.date_from < date_op < row.date_to');
        return;
      end if;
      if row.date_from<date_ed and row.date_to>date_ed then
        dbms_output.put_line('row.date_from < date_ed < row.date_to');
        return;
      end if;
    end loop;
    insert into ContractInfo values (name, date_op, date_ed, dur);
  end protemp;
/

-- 3
col title format a22;
col createdby format a12;
col label format a5;
col album_id format a8;
create table temp as
  select * from Album
  left join VinylAlbum on VinylAlbum.v_album_ID=Album.album_ID
  left join TapeAlbum on TapeAlbum.t_album_ID=Album.album_ID
  left join CDAlbum on CDAlbum.c_album_ID=Album.album_ID;
alter table temp add is_distributed_as VARCHAR2(17);
update temp set is_distributed_as='v' where v_album_ID is not null;
update temp set is_distributed_as='t' where t_album_ID is not null;
update temp set is_distributed_as='c' where c_album_ID is not null;
create or replace view AlbumDistribution as
select album_id, title, createdby, is_distributed_as from temp;
select * from temp_V;

-- 4
create or replace procedure tracttemp (name VARCHAR2, type VARCHAR2) as
  cursor con_cur is
    select title, is_distributed_as from AlbumDistribution;
  begin
    if date_op < date_ed then
      dbms_output.put_line('date_op < date_ed');
      return;
    end if;
    for row in con_cur loop
      if row.date_from<date_op and row.date_to>date_op then
        dbms_output.put_line('row.date_from < date_op < row.date_to');
        return;
      end if;
      if row.date_from<date_ed and row.date_to>date_ed then
        dbms_output.put_line('row.date_from < date_ed < row.date_to');
        return;
      end if;
    end loop;
    insert into ContractInfo values (name, date_op, date_ed, dur);
  end protemp;
/

drop table temp;

-- (e)
-- case for John Brooker is not printed as he or she does not even has a cart.
create or replace view ca_cu as
  select customerInfo.loginName, count(orderCartInfo.orderCartId) as cnt_id
  from customerInfo, orderCartInfo
  where customerInfo.loginName=orderCartInfo.customerID
  group by customerInfo.loginName;
create or replace view ca_per_cu as
  select sum(cnt_id)/count(distinct loginName)
  as res
  from ca_cu;
select loginName, case
when cnt_id<=2 then 'BR-2 satisfied'
when cnt_id>2 then 'BR-2 violated'
end cnt_id
from ca_cu;
drop view ca_cu;
drop view ca_per_cu;
-- (f)
-- select *
-- from (
--   with temp as(
--     select count(*) as count, itemNum, itemSize, itemColor
--     from inventoryItem
--     group by itemNum, itemSize, itemColor having count(*)>0
--   )select itemNum, itemSize, itemColor, case
--   when count>1 then 'BR-2 violated'
--   when count=1 then 'BR-2 satisfied'
--   end count
--   from temp
-- )where count='BR-2 violated';

select *
from(
  select itemNum, itemSize, itemColor, case
  when count>1 then 'BR-2 violated'
  when count=1 then 'BR-2 satisfied'
  end count
  from(
    select count(*) as count, itemNum, itemSize, itemColor
    from inventoryItem
    group by itemNum, itemSize, itemColor having count(*)>0
  )
)where count='BR-2 violated';


-- (g)
create or replace trigger temp
  before insert or update on itemType
  for each row
  declare
    tmp_val  float(126);
  PRAGMA AUTONOMOUS_TRANSACTION;
  begin
  select min(price) into tmp_val from itemType;
    if :new.price>4*tmp_val then
        raise_application_error(-20000,'ERROR TRIGGERED!');
    end if;
  end;
/

select * from itemType;

insert into itemType (price) values(170);
col name format a25;
col picture format a7;
select * from itemType;

update itemType set price=170 where price=15.99;
select * from itemType;

drop trigger temp;
-- [close]

start /opt/info/courses/COMP23111/drop-Eclectic-Ecommerce-tables.sql

-- [footer]
--
-- End of Exercise 05 by Haorui Chen
SPOOL OFF
