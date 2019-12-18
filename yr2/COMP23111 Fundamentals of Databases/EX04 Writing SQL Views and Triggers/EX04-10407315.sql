-- [header]
--
-- COMP23111 Fundamentals of Databases
-- Exercise 04
-- by Haorui Chen, ID 10407315, login name z26937hc

-- [opening]
SET ECHO ON -- causes the SQL statements themselves to be spooled
SPOOL EX04-10407315.log
-- sends everything to EX04-10407315.log
-- here you can set the SQL*Plus parameters, such as column width,
-- that will allow the script to produce readable answers in the spool
-- file

-- [body]
start /opt/info/courses/COMP23111/create-Eclectic-Ecommerce-tables.sql
start /opt/info/courses/COMP23111/populate-Eclectic-Ecommerce-tables.sql

-- (a)
create or replace view cast_cart as
  select firstName, lastName
  from customerInfo
  where customerInfo.loginName in(
    select customerID
    from orderCartInfo
  );
select * from cast_cart;
drop view cast_cart;

-- (b)
create or replace view reorder_needed as
  select inventoryItem.code, inventoryItem.itemNum, itemType.belongsTo, inventoryItem.qtyInstock
  from inventoryItem, itemType
  where inventoryItem.qtyInstock<25
  and inventoryItem.itemNum = itemType.itemNum;
select * from reorder_needed;
drop view reorder_needed;

-- (c)
create or replace view id_ttl as
  select orderCartInfo.orderCartId, sum(lineItems.orderPrice*lineItems.qtyOrdered) as total_price
  from orderCartInfo, lineItems
  where orderCartInfo.orderCartId=lineItems.orderCartId
  group by orderCartInfo.orderCartId;
create or replace view ord_cust_ttl as
  select customerInfo.loginName, customerInfo.firstName,
  customerInfo.lastName, id_ttl.orderCartId, id_ttl.total_price
  from customerInfo, id_ttl, orderCartInfo
  where customerInfo.loginName=orderCartInfo.customerID
  and orderCartInfo.orderCartId=id_ttl.orderCartId;
select * from ord_cust_ttl;
drop view id_ttl;
drop view ord_cust_ttl;

-- (d)
-- total_price for John Brooker with loginName choizeznyc is not shown as he/she
-- does not even have a cart.
create or replace view id_ttl as
  select customerInfo.loginName, sum(lineItems.orderPrice*lineItems.qtyOrdered)
  as total_price
  from customerInfo
  left join orderCartInfo
  on customerInfo.loginName=orderCartInfo.customerID
  left join lineItems
  on orderCartInfo.orderCartId=lineItems.orderCartId
  group by customerInfo.loginName;
create or replace view ord_cust_ttl as
 select customerInfo.loginName, customerInfo.firstName,
 customerInfo.lastName, id_ttl.total_price
 from customerInfo, id_ttl where customerInfo.loginName=id_ttl.loginName;
select * from ord_cust_ttl;
drop view id_ttl;
drop view ord_cust_ttl;

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
-- End of Exercise 04 by Haorui Chen
SPOOL OFF
