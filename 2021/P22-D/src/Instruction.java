import com.conversantmedia.util.collection.geometry.Rect3d;

public class Instruction {
    private boolean on;
    private Rect3d region;

    public Instruction(boolean on, int x1, int y1, int z1, int x2, int y2, int z2) {
        this.on = on;
        this.region = new Rect3d(x1, y1, z1, x2, y2, z2);
    }

    public boolean getOn() {
        return this.on;
    }

    public Rect3d getRegion() {
        return this.region;
    }
}
