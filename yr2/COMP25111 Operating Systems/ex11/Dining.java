public class Dining
{
  // You will change this value in STAGE *FOUR* (and FIVE).
  //public final static boolean AVOID_DEADLOCK = true;
  public final static boolean AVOID_DEADLOCK = false;


  // You will change this value in STAGE *FIVE*
  // (and set AVOID_DEADLOCK back to false too).
  //public static boolean DELAY = false;
  public static boolean DELAY = true;

  // You will change this value in STAGE *FIVE*
  // (set it to -1 for `unlimited'. Well, 2^32...)
  //public final static int NUMBER_OF_CYCLES = -1;
  public final static int NUMBER_OF_CYCLES = 3;


  // Optionally you can change this number, but you don't want too much output
  // and 2 is boring. (And 1 would be pointless!)
  private static final int NUMBER_OF_DINERS = 3;


  // The main method creates the diners and forks as we go `around' the table.
 // And sets each diner going.
  public static void main(String[] args)
  {
    Fork firstFork = new Fork();
    Fork nextLeft = firstFork;

    // Loop around the table, but stop before the final diner.
    // Start each diner as we create them, so that we don't need to have a data
    // structure to store them in!
    for (int dinerCount = 1; dinerCount < NUMBER_OF_DINERS; dinerCount++)
    {
      // Each iteration creates one fork, so we will have NUMBER_OF_DINERS
      // forks by the end of the loop.
      Fork thisRight = new Fork();

      // Start the diner (thread) immediately after creation.
      new Diner(nextLeft, thisRight).start();

      // The next diner must use this right fork on their left.
      nextLeft = thisRight;
    } // for

    // The final diner uses the last fork created on their left, and the first
    // on their right.
    new Diner(nextLeft, firstFork).start();
  } // main

} // class Dining
