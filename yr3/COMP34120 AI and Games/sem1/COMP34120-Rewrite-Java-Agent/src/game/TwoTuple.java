package game;

public class TwoTuple implements Comparable{
    private int choice;
    private int weight;

    public TwoTuple(int choice, int weight) {
        this.choice = choice;
        this.weight = weight;
    }

    public int getChoice() {
        return choice;
    }


    public int getWeight() {
        return weight;
    }


    @Override
    public int compareTo(Object o) {
        TwoTuple s = (TwoTuple)o;
        if(this.weight < s.weight){
            return -1;
        }else if(this.weight == s.weight){
            return 0;
        }else {
            return -1;
        }
    }
}
