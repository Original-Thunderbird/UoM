/*

Each instance of this class represents a diner.

Each has a unique id number which is used for identification.

The class is a subclass of Thread, because we wish each diner to act
independently with all of them acting at the same time.

*/
public class Diner extends Thread
{
  // Count of instances used to award a unique id to each diner.
  private static int instanceCountSoFar = 0;

  // Each diner has a unique id, which is the count of diners created so far.
  private final int idNo;

  // Each diner has a fork on each side, which they can attempt to pick up and
  // put down.
  private final Fork left, right;


  // Constructor is given the two forks to be remembered, and ensures unique
  // id by counting instances.
  public Diner(Fork requiredLeft, Fork requiredRight)
  {
    instanceCountSoFar++;
    idNo = instanceCountSoFar;
    left = requiredLeft;
    right = requiredRight;
  } // Diner


  // Thread has a method run which is called by its start method.
  // When we make a subclass of Thread we typically override run to implement
  // what we really want to happen.
  @Override
  public void run()
  {
    // The diner can be set up to pick up the forks in either order.
    Fork firstToPickUp = left, secondToPickUp = right;

    // Can you guess what this code is for?
    if (Dining.AVOID_DEADLOCK && left.compareTo(right) > 0)
    {
      firstToPickUp = right;
      secondToPickUp = left;
    } // if

    // The diners loop goes round a number of times, with each iteration
    // having a thinking then eating stage.

    // Loop control needs to be `endless' if Dining.NUMBER_OF_CYCLES is -1.
    for (int cycleNumber = 1; cycleNumber != Dining.NUMBER_OF_CYCLES + 1;
         cycleNumber++)
    {
      // Thinking first. This will take one second if delay is enabled.
      Logger.log(this + " is thinking at cycle " + cycleNumber);

      // Now eating (with a one second delay for each log if enabled).
      Logger.log(this + " wants to pick up first " + firstToPickUp);

      // In STAGE *TWO* you will uncomment this synchronized.
      // Hit <enter> here ->      synchronized(firstToPickUp)
      {

        // Attempt to pick up the first fork, which will be logged by the
        // fork. This could fail of course, but the diner would no know!
        firstToPickUp.pickUp(this);
        Logger.log(this + " thinks has picked up first " + firstToPickUp);


        // Now the second fork.
        Logger.log(this + " wants to pick up second " + secondToPickUp);
        // In STAGE *THREE* you will uncomment this synchronized.
        // Hit <enter> here ->        synchronized(secondToPickUp)
        {
          // Attempt to pick up the second fork which could (also) fail.
          secondToPickUp.pickUp(this);
          Logger.log(this + " thinks has picked up second " + secondToPickUp);

          // The diner thinks s/he has two forks, but s/he might have one or
          // none! Goodness knows how s/he eats in those cases, perhaps s/he
          // goes through the actions of using both forks but fails to eat.
          Logger.log(this + " is eating at cycle " + cycleNumber);

          // The forks are put down in the reverse order they were picked up
          // in. Why?
          secondToPickUp.putDown(this);
        } // synchronised secondToPickUp

        firstToPickUp.putDown(this);
      } // synchronised firstToPickUp
    } // for
  } // run


  // The toString just identifies the diner. (Showing the forks would be two
  // cluttered.
  @Override
  public String toString()
  {
    return "Diner " + idNo;
  } // toString

} // class Diner
