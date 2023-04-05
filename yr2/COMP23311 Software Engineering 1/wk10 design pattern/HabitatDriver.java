
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Random;

public class HabitatDriver {

  private ArrayList<Habitat> habitats = new ArrayList<Habitat>();

  public static void main(String[] args) {
    HabitatDriver driver = new HabitatDriver();
  }

  public HabitatDriver() {

    Habitat theFarm = new Farm();
    Pet petGoat = new Goat();
    theFarm.addOccupant(petGoat);
    habitats.add(theFarm);

    Habitat cottage = new CosyCottage();
    Pet petCat = new Cat();
    cottage.addOccupant(petCat);
    habitats.add(cottage);

    Habitat theCaves = new MythicalCaveSystem();
    Pet petDragon = new MagicDragon();
    theCaves.addOccupant(petDragon);
    habitats.add(theCaves);

    for (Habitat habitat: habitats) {
      System.out.println(habitat.describe());
      System.out.println("Contains: " + habitat.getOccupants().toString());
      System.out.println("");
    }

    FieryMountains mountains = new FieryMountains();
    System.out.println(mountains.getDescription().getText());
    System.out.println("Population: " + String.valueOf(mountains.getDescription().getPopulation()));

  }

}
