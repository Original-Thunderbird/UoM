
import java.util.Arrays;
import java.util.List;
import java.util.Random;


public class PetDriver {

  private static List<String> foodItems = Arrays.asList("chicken", "fish", "milk", "grass", "corn", "hats", "socks", "ham", "pizza", "meat", "coal");

  public static void main(String[] args) {
    PetDriver driver = new PetDriver();
  }

  public PetDriver() {
    Pet petCat = new Cat();
    Pet petDragon = new MagicDragon();
    Pet petGoat = new Goat();
    for (int i = 0; i<70; ++i) {
      System.out.println(petCat.describe());
      System.out.println(petGoat.describe());
      System.out.println(petDragon.describe());
      petCat.timePasses();
      petGoat.timePasses();
      petDragon.timePasses();

      Random rand = new Random();
      int n = rand.nextInt(8);  // returns 0-7
      if (n == 0) {
        petCat.feed(foodItems.get(rand.nextInt(foodItems.size())));
      } else if (n == 2) {
        petGoat.feed(foodItems.get(rand.nextInt(foodItems.size())));
      } else if (n == 4) {
        petDragon.feed(foodItems.get(rand.nextInt(foodItems.size())));
      } else if (n == 6) {
        petCat.feed("chicken");
        petGoat.feed("socks");
        petDragon.feed("coal");
      }

      System.out.println("");
    }

    System.out.println(petCat.size);
    System.out.println(petGoat.size);
    System.out.println(petDragon.size);

  }

}
