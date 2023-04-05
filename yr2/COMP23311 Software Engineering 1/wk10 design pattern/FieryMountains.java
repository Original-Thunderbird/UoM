/***************************************************************************
 *                   COMP 23311 - Week 10 - Design Patterns                *
 ***************************************************************************
 ***************************************************************************
 *                                                                         *
 *   Legacy class FieryMountains could be adapted to become compatible     *
 *   with Habitat.                                                         *
 ***************************************************************************/

import java.util.List;


public class FieryMountains {

  private LocationDescriptor location;

  public FieryMountains() {
    location = new LocationDescriptor(2096, "These are the Fiery Mountains of doom.");
  }

  public LocationDescriptor getDescription() {
    return location;
  }

}

class LocationDescriptor {

  private int population;
  private String textDescription;

  public LocationDescriptor(int population, String textDescription) {
    this.population = population;
    this.textDescription = textDescription;
  }

  public int getPopulation () {
    return population;
  }

  public String getText() {
    return textDescription;
  }
}
