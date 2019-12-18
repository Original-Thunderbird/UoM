/*

Each instance of this class represents a fork.

Each has a unique id number which is used for the natural order of forks as
well as identification.

Can you think why it may be helpful to have that order?

*/

public class Fork implements Comparable<Fork>
{
  // Count of instances used to award a unique id to each fork.
  private static int instanceCountSoFar = 0;

  // Each fork has a unique id, which is the count of forks created so far.
  private final int idNo;

  // At any time, the fork maybe on the table, or be being hold by exactly one
  // diner. null means on the table.
  private Diner currentHolder = null;


  // Constructor ensures unique id by counting instances.
  public Fork()
  {
    instanceCountSoFar++;
    idNo = instanceCountSoFar;
  } // Fork


  // The fork can be picked up by a diner if it is on the table.
  // If it is already being hold, then there's no effect.
  // Except that behaviour is logged in both cases.
  public void pickUp(Diner newHolder)
  {
    if (currentHolder == null)
    {
      currentHolder = newHolder;
      Logger.log(newHolder + " is holding " + this);
    } // if
    else
    {
      Logger.log("OOOPS: " + newHolder + " cannot pick up " + this);
    } // else
  } // pickUp


  // The fork can be put down if it is being held by a diner, and it is the
  // same diner that is trying to put it down (apparentHolder).
  // Otherwise there is no effect.
  // Except that behaviour is logged in both cases.
  public void putDown(Diner apparentHolder)
  {
    if (currentHolder != apparentHolder)
      Logger.log("OOOPS: " + apparentHolder  + " cannot put down " + this);
    else
    {
      Logger.log(currentHolder + " has put down " + this);
      currentHolder = null;
    } // else
  } // pickUp


  // Natural order based on the unique ids.
  @Override
  public int compareTo(Fork other)
  {
    return idNo - other.idNo;
  } // compareTo



  // The toString shows the state, including who if anyone is holding it.
  @Override
  public String toString()
  {
    return "Fork " + idNo
           + " (held by "
                + (currentHolder == null ? "noone" : currentHolder)
                + ")";
  } // toString

} // class Fork
