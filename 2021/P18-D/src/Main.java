import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Stack;

public class Main {
    private static ArrayList<Node> getData(String file) throws IOException {
        ArrayList<Node> r = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = br.readLine()) != null) {
                Stack<Object> queue = new Stack<>();
                for (int x = 0; x < line.length(); x++) {
                    if (line.charAt(x) == ']') {
                        if (line.charAt(x-1) != ']') queue.push((byte)(line.charAt(x-1)-'0'));
                        Object last = queue.pop();
                        queue.push(Node.getNode(queue.pop(), last));
                    }
                    else if (line.charAt(x) == ',') {
                        if (line.charAt(x-1) != ']') queue.push((byte)(line.charAt(x-1)-'0'));
                    }
                }
                r.add((Node) queue.pop());
            }
        }
        return r;
    }

    public static void main(String[] args) throws IOException {
        ArrayList<Node> data = getData("input.txt");
        int max_magnitude = 0;
        ArrayList<Permutator> permutators = new ArrayList<>();
        for (Node n1 : data) {
            Permutator p = new Permutator(n1, data);
            permutators.add(p);
            p.start();
        }

        try {
            int count = 1;
            for (Permutator p : permutators) {
                p.join();
                if (p.getMaxValue() > max_magnitude) max_magnitude = p.getMaxValue();

                System.out.println(count + "/" + permutators.size());
                count++;
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println(max_magnitude);
    }
}
