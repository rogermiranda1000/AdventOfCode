import java.util.ArrayList;

public class Permutator extends Thread {
    private int max_value;
    private Node n1;
    private ArrayList<Node> n2;

    public Permutator(Node n1, ArrayList<Node> n2) {
        this.n1 = n1;
        this.n2 = n2;
    }

    public int getMaxValue() {
        return this.max_value;
    }

    @Override
    public void run() {
        int tmp;
        this.max_value = 0;

        for (Node n : this.n2) {
            if (this.n1 == n) continue;

            tmp = new Node(this.n1).add(new Node(n)).getMagnitude();
            if (tmp > this.max_value) this.max_value = tmp;
        }
    }
}
