
import java.util.Scanner;


public class Tamagotchi {

  private static Scanner scanner = new Scanner( System.in );

  public static void main(String[] args) {
    Tamagotchi driver = new Tamagotchi();
  }

  public Tamagotchi() {
    System.out.println("Hi -- Welcome to Tamagotchi. Let's create you a new pet." );
    String input = "";
    while (!(input.equals("cat") || input.equals("goat") || input.equals("dragon") || input.equals("toy"))) {
      System.out.print("What kind of pet would you like? (cat, goat, dragon or toy?) ");
      input = scanner.nextLine().toLowerCase().trim();
    }

    Pet pet = new Cat();

    System.out.println("You wanted a pet " +  input + "? Great, I've created you a brand new " + pet.toString() + ". " + pet.cry());

    while (!(input.equals("cottage") || input.equals("farm") || input.equals("caves") || input.equals("puddle"))) {
      System.out.print("Where would you like your pet to live? (cottage, farm, caves or puddle?) ");
      input = scanner.nextLine().toLowerCase().trim();
    }

    Habitat habitat = new Farm();

    System.out.println("Your pet " + pet.toString() + " lives on a farm. ");

    while (!input.equals("Q")) {
      System.out.print("What do you want to do with your pet?  You can (F)eed the pet, (W)atch the pet, or (Q)uit the Tamagotchi application. ");
      input = scanner.nextLine().trim().substring(0,1).toUpperCase();

      if (input.equals("F")) {
        System.out.print("What do you want to feed your pet? ");
        input = scanner.nextLine().toLowerCase().trim();
        pet.feed(input);
      } else if (input.equals("W")) {
        pet.timePasses();
        System.out.println(pet.describe());
      }
    }

  }

}
