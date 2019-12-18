/*

This class contains one static method used to `log' the behaviour of the
program onto standard output.

Each message is presented with the message count (from 1 upwards) and the time
in second since the program was loaded.

There will then be a delay of one second if Dining.DELAY has the value true.
This is a cheap way of slowing down our threads if we wish to.

Note the use of an Object as a lock on the critical section of code. There is
a reason why we have not used synchronized on the entire log method -- can you
think what that might be? (Hint -- what would happen if the sleep delay was
part of the critical section?)

*/

public class Logger
{

  // Store the system time at the start so we know the elapsed for each
  // message as it is output.
  private static final long START_TIME = System.currentTimeMillis();

  // Each message has a unique count.
  private static int messageCount = 0;

  // We need to ensure that only one thread is running the critical section of
  // incrementing the message count and outputting the message.
  // We will use this variable to refer to an object for that lock.
  private static Object lock = new Object();


  // Log one message.
  public static void log(String message)
  {
    // The time now in seconds since the start of the program.
    long timeNow = (System.currentTimeMillis() - START_TIME) / 1000;

    synchronized(lock)
    {
      messageCount++;
      System.out.println(messageCount + "@" + timeNow + ": " + message);
    } // synchronized

    // Optionally sleep for one second.
    if (Dining.DELAY)
      try { Thread.sleep(1000); } catch (InterruptedException e) {}
  } // log

} // class Logger
