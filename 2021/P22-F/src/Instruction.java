import com.conversantmedia.util.collection.geometry.Rect3d;

public class Instruction {
    private boolean on, easy;
    private Rect3d region;

    public Instruction(boolean on, int x1, int y1, int z1, int x2, int y2, int z2) {
        // TODO la regió ha d'anar de -50 a +50
        this.easy = x1 >= -50 && y1 >= -50 && z1 >= -50 && x2 <= 50 && y2 <= 50 && z2 <= 50;
        this.on = on;
        this.region = new Rect3d(x1, y1, z1, x2, y2, z2);
    }

    public boolean getEasy() {
        return this.easy;
    }

    public boolean getOn() {
        return this.on;
    }

    public Rect3d getRegion() {
        return this.region;
    }
}
