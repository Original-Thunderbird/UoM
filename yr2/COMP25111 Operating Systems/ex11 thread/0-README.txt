Welcome to this exercise about threads. It's all based on the classic dining
philosophers, in which N diners are sat around a circular table and wish to
alternate between thinking and eating. To eat, a diner needs to hold two
forks, which can be picked up from the table, on their left and right
respectively. The snag is, there are only N forks, each to be shared by the
two diners on either side of it!

Good luck!

Thanks, John

------------------------------------------------------------------------------
STAGE ZERO:

Your first job is to read the code, making notes for your own memory, and any
guesses at the few questions in comments, in your ~/COMP25111/ex11/ex11.txt
file (which will become your submission).

I recommend reading the code in this order:

	Logger.java
	Fork.java
	Diner.java
	Dining.java

------------------------------------------------------------------------------
STAGE ONE:

Compile and run the program as is. Use:

		javac Dining.java
		java Dining | less

Explain the behaviour in ex11.txt.

------------------------------------------------------------------------------
STAGE TWO:

Uncomment the first synchronized in Diner.java, recompile, rerun and explain
the change in behaviour.

------------------------------------------------------------------------------
STAGE THREE:

Now uncomment the second synchronized in Diner.java and recompile, Rerun
without using less this time.

You might want to wonder how long to wait before hitting control-C. Explain
the behaviour. In particular, why was STAGE TWO not sufficient to achieve this
effect?

------------------------------------------------------------------------------
STAGE FOUR:

Change Dining.AVOID_DEADLOCK to true and recompile. You will need to recompile
Diner.java too.

Rerun through less again and explain the behaviour. Run it yet again -- is it
different? Try a few times. E.g.:

	for n in `seq 1 5`
	do
	  java Dining > test-$n
	done

(That should take about 5 minutes, so find something useful to do.)

Then maybe:

	ls -l test-*

might show something.

Then, e.g.:

	diff test-[12]

etc.. If you are lucky, then

	tail -n 1 test-*

might show a significant difference (who was last to finish?).

If you are determined to see that change, you could try this:

	for n in `seq 1 1000`
	do
	  java Dining > test-$n
	  tail -n 1 test-*
	  echo
	done

which should show something every minute and you can control-C when you see
what you are looking for, or want to give up waiting. In fact, better than
control-C (which you would have to keep pressed down) would be:

	control-Z

	kill %1

which suspends the loop (to become job 1) and then kills that job (%1).

------------------------------------------------------------------------------
STAGE FIVE:

Change Dining.AVOID_DEADLOCK back to false, Dining.NUMBER_OF_CYCLES to -1 and
Dining.DELAY to false, then recompile. You will also need to recompile
Diner.java.

Run the program several times (you will need control-C), and explain the
behaviour.

------------------------------------------------------------------------------
