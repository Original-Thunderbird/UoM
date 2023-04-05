/***************************************************************************
 *                   COMP 23311 - Week 10 - Design Patterns                *
 ***************************************************************************
 ***************************************************************************
 *                                                                         *
 *   Some simple habitats for Pets, to be refactored with design patterns  *
 ***************************************************************************/

import java.util.ArrayList;


public abstract class Habitat {

  private ArrayList<Pet> occupants;

  public Habitat() {
    occupants = new ArrayList<Pet>();
  }

  public int addOccupant(Pet occupant) {
    if (!occupants.contains(occupant)) {
      occupants.add(occupant);
    }
    return occupants.size();
  }

  public Pet[] getOccupants() {
    return (Pet[])occupants.toArray();
  }

  public abstract String describe();

  public int removeOccupant(Pet occupant) {
    occupants.remove(occupant);
    return occupants.size();
  }

}


// cats live in cottages
class CosyCottage extends Habitat {
  public String describe() {
    return "This is a cosy cottage. Small on the outside, smaller on the inside.";
  }
}


// goats live on farms
class Farm extends Habitat {
  public String describe() {
    return "This is Grandpa Joe's farm. Two small fields and a barn.";
  }
}


// dragons live in mythical caves
class MythicalCaveSystem extends Habitat {
  public String describe() {
    return "These are mythical caves. There appear to be three caves in total, each one larger and darker than the rest.";
  }
}
