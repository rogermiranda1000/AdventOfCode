import com.conversantmedia.util.collection.geometry.Rect3d;
import com.conversantmedia.util.collection.spatial.SpatialSearch;
import com.conversantmedia.util.collection.spatial.SpatialSearches;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Scanner;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    private static Instruction[] getInstructions(String file) throws FileNotFoundException {
        ArrayList<Instruction> instructions = new ArrayList<>();

        Scanner sc = new Scanner(new File(file));
        String digit = "[\\-0-9]+";
        Pattern pattern = Pattern.compile("(on|off) x=(" + digit + ")\\.\\.(" + digit + "),y=(" + digit + ")\\.\\.(" + digit +
                "),z=(" + digit + ")\\.\\.(" + digit + ")");

        while (sc.hasNextLine()) {
            Matcher matcher = pattern.matcher(sc.nextLine());
            matcher.find();
            instructions.add(new Instruction(matcher.group(1).equals("on"),

                    Integer.parseInt(matcher.group(2)),
                    Integer.parseInt(matcher.group(4)),
                    Integer.parseInt(matcher.group(6)),

                    Integer.parseInt(matcher.group(3)),
                    Integer.parseInt(matcher.group(5)),
                    Integer.parseInt(matcher.group(7))
                ));
            System.out.println();
        }

        sc.close();

        return instructions.toArray(new Instruction[0]);
    }

    public static double []getCoordinates(Rect3d rect) {
        double []r = new double[6];
        for (int n = 0; n < 3; n++) r[n] = rect.getMin().getCoord(n);
        for (int n = 0; n < 3; n++) r[n+3] = rect.getMax().getCoord(n);
        return r;
    }

    public static void removeInnerRectangle(SpatialSearch<Rect3d> space, Rect3d outer, Rect3d remove) {
        space.remove(outer);

        double []outerRectangle = getCoordinates(outer);
        double []removeRectangle = getCoordinates(remove);
        if (outerRectangle[1] < removeRectangle[1]) space.add(new Rect3d(outerRectangle[0], outerRectangle[1], outerRectangle[2], outerRectangle[3], removeRectangle[1]-1, outerRectangle[5])); // top rectangle
        if (outerRectangle[4] > removeRectangle[4]) space.add(new Rect3d(outerRectangle[0], removeRectangle[4]+1, outerRectangle[2], outerRectangle[3], outerRectangle[4], outerRectangle[5])); // bottom rectangle
        if (outerRectangle[0] < removeRectangle[0]) space.add(new Rect3d(outerRectangle[0], Math.max(removeRectangle[1], outerRectangle[1]), outerRectangle[2],
                removeRectangle[0]-1, Math.min(removeRectangle[4], outerRectangle[4]), outerRectangle[5])); // left rectangle
        if (outerRectangle[3] > removeRectangle[3]) space.add(new Rect3d(removeRectangle[3]+1, Math.max(removeRectangle[1], outerRectangle[1]), outerRectangle[2],
                outerRectangle[3], Math.min(removeRectangle[4], outerRectangle[4]), outerRectangle[5])); // right rectangle
        if (outerRectangle[2] < removeRectangle[2]) space.add(new Rect3d(Math.max(removeRectangle[0], outerRectangle[0]), Math.max(removeRectangle[1], outerRectangle[1]), outerRectangle[2],
                Math.min(removeRectangle[3], outerRectangle[3]), Math.min(removeRectangle[4], outerRectangle[4]), removeRectangle[2]-1));
        if (outerRectangle[5] > removeRectangle[5]) space.add(new Rect3d(Math.max(removeRectangle[0], outerRectangle[0]), Math.max(removeRectangle[1], outerRectangle[1]), removeRectangle[5]+1,
                Math.min(removeRectangle[3], outerRectangle[3]), Math.min(removeRectangle[4], outerRectangle[4]), outerRectangle[5]));
    }

    public static void subtractRectangle(SpatialSearch<Rect3d> space, Rect3d remove) {
        Collection<Rect3d> rectangles = new ArrayList<>();
        space.intersects(remove, (e)->rectangles.add(e));
        for (Rect3d collision : rectangles) removeInnerRectangle(space, collision, remove);
    }

    public static int countRectangles(SpatialSearch<Rect3d> space) {
        final AtomicInteger r = new AtomicInteger();
        space.forEach((rect) -> {
            int tmp = 1;
            for (int x = 0; x < 3; x++) tmp *= (rect.getRange(x)+1);
            r.addAndGet(tmp);

            //System.out.println(rect.toString() + ": " + tmp);
        });
        return r.get();
    }

    public static void main(String[] args) throws FileNotFoundException {
        Instruction[] instructions = getInstructions("input.txt");
        SpatialSearch<Rect3d> space = SpatialSearches.rTree(new Rect3d.Builder());
        for (Instruction ins : instructions) {
            if (!ins.getEasy()) continue;

            if (ins.getOn()) {
                subtractRectangle(space, ins.getRegion()); // evita que un punt estigui a dos rectangles diferents; optimitza per fer després el comptatge
                space.add(ins.getRegion());
            }
            else subtractRectangle(space, ins.getRegion());
        }

        System.out.println(countRectangles(space));
    }
}
