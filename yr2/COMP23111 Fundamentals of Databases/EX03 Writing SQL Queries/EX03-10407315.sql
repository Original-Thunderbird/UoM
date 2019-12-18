-- [header]
--
-- COMP23111 Fundamentals of Databases
-- Exercise 03
-- by Haorui Chen, ID 10407315, login name z26937hc

-- [opening]
SET ECHO ON -- causes the SQL statements themselves to be spooled
SPOOL EX03-10407315.log
-- sends everything to <spoolfilename>
-- here you can set the SQL*Plus parameters, such as column width,
-- that will allow the script to produce readable answers in the spool
-- file

-- [body]
start /opt/info/courses/COMP23111/create-University-tables.sql
start /opt/info/courses/COMP23111/populate-University-tables.sql

-- 1 (a) i
select distinct student.name
from student
inner join takes
on student.ID=takes.ID
join course
on takes.course_id=course.course_id
where course.dept_name='Comp. Sci.';

-- 1 (a) ii
select ID, name
from student
where ID not in(
  select ID
  from takes
  where year<2009
);

-- 1 (a) iii
create view inst_sal as
select max(instructor.salary) as max_salary, dept_name
from instructor group by dept_name;
select *
from inst_sal;

-- 1 (a) iv
select min(inst_sal.max_salary)
from inst_sal;
drop view inst_sal;

-- 1 (b) i
insert into course (course_id, title, dept_name, credits)
values ( 'CS-001', 'Weekly Seminar', 'Comp. Sci.', 10);

--1 (b) ii, iii
-- ERROR ORA-02290: check constraint violated
-- <document>
-- Cause: The values being inserted do not satisfy the named check constraint.
-- Action: do not insert values that violate the constraint.
--<code>
-- create table course (
--    ......
--        credits	     numeric(2,0)
--        			check (credits > 0),   rule violated
--    ......
-- );
-- insert into course (course_id, title, dept_name, credits)
-- values ( 'CS-002', 'Monthly Seminar', 'Comp. Sci.', 0);

--1 (b) iv, v(?)
-- <ducoment>
-- If a column in a row has no value, then the column is said to be null, or to
-- contain null.
-- So the missing columns contain NULL.
insert into section (course_id, sec_id, semester, year)
values ('CS-001', '1', 'Fall', 2009);

--1 (b) vi
insert into takes (ID, course_id, sec_id, semester, year)
select student.ID, section.course_id, section.sec_id, section.semester, section.year
from student, section
where student.dept_name='Comp. Sci.'
and section.course_id = 'CS-001';

--1 (b) vii
delete takes
where course_ID='CS-001'
and ID in (
select distinct ID
from student
where name='Zhang');

--1 (b) viii
delete takes where course_id in(
select course_id from course where instr(lower(course.title), 'database')>0);

--1 (b) ix,x
--<code>
-- create table section (
--            ......
--        foreign key (course_id)
--        	       references course
--        	       on delete cascade,
--            ......
-- );
-- create table takes (
--             ......
--        foreign key (course_id,sec_id, semester, year)
--        	       references section
--        	       on delete cascade,
--             ......
-- );
-- It means that when a course is deleted from course relation, tuples in
-- sections relation related to the course are deleted accordingly, then tuples
-- in takes relation relation to those sections are deleted accordingly.
delete course
where course_id='CS-001'

start /opt/info/courses/COMP23111/drop-University-tables.sql

start /opt/info/courses/COMP23111/create-Accident-tables.sql
start /opt/info/courses/COMP23111/populate-Accident-tables.sql

-- 2 (a) i
select count(report_number)
from participated
join person
on participated.driver_id=person.driver_id
where person.name='Jane Rowling';

-- 2 (a) ii
update participated
set damage_amount=2500
where report_number=7897423
and license='KUY 629';

-- 2 (a) iii
with temp as (
select driver_id, sum(damage_amount)
as total_damage
from participated
group by driver_id
)select person.name, temp.total_damage
from person, temp
where person.driver_id=temp.driver_id
and temp.total_damage>3000
order by temp.total_damage desc;

-- 2 (a) iv

create view damage_per_location as
select accident.location, participated.damage_amount
from accident, participated
where accident.report_number=participated.report_number;
create view average_damage_per_location as
select location, avg(damage_amount) as av_amt
from damage_per_location
group by location;
select *
from average_damage_per_location;

--2 (a) v (guess)
select location from average_damage_per_location
where av_amt=(select max(av_amt) from average_damage_per_location);
drop view damage_per_location;
drop view average_damage_per_location;

-- [close]
start /opt/info/courses/COMP23111/drop-Accident-tables.sql
-- [footer]
--
-- End of Exercise 03 by Haorui Chen
SPOOL OFF
