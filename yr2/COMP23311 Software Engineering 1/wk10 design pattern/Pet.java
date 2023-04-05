/***************************************************************************
 *                   COMP 23311 - Week 10 - Design Patterns                *
 ***************************************************************************
 ***************************************************************************
 *                                                                         *
 *   Simple "Stendhal-inspired" Pets to be refactored with design patterns *
 ***************************************************************************/

import java.util.Arrays;
import java.util.List;
import java.util.Random;


public abstract class Pet {
    protected static final int HUNGER_HUNGRY = 300;
    protected static final int HUNGER_STARVATION = 750;
    private static final int START_HUNGER_VALUE = 0;

    private static final int START_SIZE = 0;
    protected static final int ADULT_SIZE = 500;

    protected int GROWTH_INTERVAL;

    protected List<String> ACCEPTABLE_FOODS;

    protected int hunger;
    protected int size;


    public Pet() {
      hunger = START_HUNGER_VALUE;
      size = START_SIZE;
    }

    public boolean canGrow() {
      return !isGrown()  && !isHungry();
    }

    public abstract String cry();

    public String describe() {
  		return "You see a cute " + toString();
  	}

    public void feed(String food) {
      if (ACCEPTABLE_FOODS.contains(food.toLowerCase())) {
        System.out.println("Your " + toString() + " has eaten some " + food + ".");
  		  hunger = START_HUNGER_VALUE;
      } else {
        System.out.println("Your " + toString() + " refuses to eat the " + food + ".");
      }
  	}

    public String getHunger() {
      if (hunger > HUNGER_STARVATION) {
        return "starving";
      } else if (isHungry()) {
        return "hungry";
      }
      return "content";
    }

    public void grow() {
      if (canGrow()) {
        size += GROWTH_INTERVAL;
      }
    }

    protected boolean isGrown() {
      return size >= ADULT_SIZE;
    }

    public boolean isHungry() {
      return hunger > HUNGER_HUNGRY;
    }

    public void timePasses() {
      hunger += 20;
      if (isHungry()) {
        System.out.println(cry() + " Your " + toString() + " is " + getHunger());
      }
      grow();
    }

    public String toString() {
      return this.getClass().getCanonicalName().toLowerCase();
    }

}


class Cat extends Pet {

    public Cat() {
      super();
      GROWTH_INTERVAL = 5;  // Cats grow pretty slowly
      ACCEPTABLE_FOODS = Arrays.asList("chicken", "fish", "milk");
    }

    public String cry() {
      return "Meow!";
    }

    public String toString() {
      if (!isGrown()) {
        return "kitten";
      }
      return super.toString();
    }

}


class Goat extends Pet {

    public Goat() {
      super();
      GROWTH_INTERVAL = 15;  // Goats grow faster than cats
      ACCEPTABLE_FOODS = Arrays.asList("socks", "hats", "grass", "corn");
    }

    public String cry() {
      return "Bleat!";
    }

    public String toString() {
      if (!isGrown()) {
        return "kid (goat, not human)";
      }
      return super.toString();
    }

}


class MagicDragon extends Pet {

    protected static final int SMALL_SIZE = 100;
    protected static final int MODERATE_SIZE = 300;

    public MagicDragon() {
      super();
      GROWTH_INTERVAL = 25;  // Dragons grow in leaps and bounds
      ACCEPTABLE_FOODS = Arrays.asList("ham", "pizza", "meat", "coal");
    }

    public String cry() {
      return "Roar!";
    }

    public void grow() {
      if (canGrow()) {
        // Dragons grow kinda randomly
        Random rand = new Random();
        int  n = rand.nextInt(8);  // returns 0-7
        if (n == 7) {
          size += GROWTH_INTERVAL;
        }
      }
    }

    public String toString() {
      String qualifier = "";
      if (size < SMALL_SIZE) {
        qualifier = "tiny ";
      } else if (size  < MODERATE_SIZE) {
        qualifier = "small ";
      } else if (!isGrown()) {
        qualifier = "moderately-sized ";
      } else {
        qualifier = "fully grown";
      }
      return qualifier + "dragon";
    }

}
