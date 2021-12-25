import org.jetbrains.annotations.Nullable;

public class Node {
    private static final int EXPLODE_DEPTH = 4;

    private Node father;
    private Node left_node, right_node;
    private byte left_value, right_value;

    public Node(Node that) {
        this.left_value = that.left_value;
        this.right_value = that.right_value;
        if (that.left_node != null) {
            this.left_node = new Node(that.left_node);
            this.left_node.father = this;
        }
        if (that.right_node != null) {
            this.right_node = new Node(that.right_node);
            this.right_node.father = this;
        }
    }

    public Node(byte left_value, byte right_value) {
        this.left_value = left_value;
        this.right_value = right_value;
        this.left_node = null;
        this.right_node = null;
    }

    public Node(Node left_node, byte right_value) {
        this.left_node = left_node;
        this.right_node = null;
        this.right_value = right_value;

        left_node.setFather(this);
    }

    public Node(byte left_value, Node right_node) {
        this.left_value = left_value;
        this.left_node = null;
        this.right_node = right_node;

        right_node.setFather(this);
    }

    public Node(Node left_node, Node right_node) {
        this.left_node = left_node;
        this.right_node = right_node;

        left_node.setFather(this);
        right_node.setFather(this);
    }

    public static Node getNode(Object remove, Object last) {
        if (remove instanceof Node) {
            if (last instanceof Node) return new Node((Node) remove, (Node) last);
            else return new Node((Node) remove, (byte) last);
        }
        else {
            if (last instanceof Node) return new Node((byte) remove, (Node) last);
            else return new Node((byte) remove, (byte) last);
        }
    }

    public void setFather(Node n) {
        this.father = n;
    }

    public Node getFather() {
        return this.father;
    }

    public void addLeftValue(byte val) {
        this.left_value += val;
    }

    public byte getLeftValue() {
        return this.left_value;
    }

    public void addRightValue(byte val) {
        this.right_value += val;
    }

    public byte getRightValue() {
        return this.right_value;
    }

    public void removeSon(Node remove) {
        if (this.left_node == remove) this.left_node = null;
        else if (this.right_node == remove) this.right_node = null;
    }

    private void addValueOFNodeWithValueAtLeft(@Nullable Node pre, byte value) {
        if (pre != null) {
            // s'està anant a l'esquerra
            if (this.left_node == null) this.left_value += value; // s'ha trobat un vàl·lid
            else if (this.left_node != pre) this.left_node.addValueOFNodeWithValueAtLeft(null, value);
            else {
                if (this.father == null) return; // no hi ha cap
                this.father.addValueOFNodeWithValueAtLeft(this, value);
            }
        }
        else {
            Node n = this.getFirstRightValueNode();
            if (n != null) n.right_value += value;
        }
    }

    private void addValueOFNodeWithValueAtRight(@Nullable Node pre, byte value) {
        if (pre != null) {
            // s'està anant a la dreta
            if (this.right_node == null) this.right_value += value; // s'ha trobat un vàl·lid
            else if (this.right_node != pre) this.right_node.addValueOFNodeWithValueAtRight(null, value);
            else {
                if (this.father == null) return; // no hi ha cap
                this.father.addValueOFNodeWithValueAtRight(this, value);
            }
        }
        else {
            Node n = this.getFirstLeftValueNode();
            if (n != null) n.left_value += value;
        }
    }

    @Nullable
    public Node getLeftmost(int depth) {
        Node r = null;
        if (this.left_node != null) r = this.left_node.getLeftmost(depth-1);
        if (r == null && this.right_node != null) r = this.right_node.getLeftmost(depth-1);
        if (r == null && this.left_node == null && this.right_node == null && depth <= 0) r = this;
        return r;
    }

    public Node getFirstRightValueNode() {
        if (this.right_node != null) return this.right_node.getFirstRightValueNode();
        return this;
    }

    public Node getFirstLeftValueNode() {
        if (this.left_node != null) return this.left_node.getFirstLeftValueNode();
        return this;
    }

    public boolean explode() {
        Node explode = this.getLeftmost(Node.EXPLODE_DEPTH);
        if (explode == null) return false;

        while(explode != null) {
            explode.getFather().addValueOFNodeWithValueAtLeft(explode, explode.getLeftValue());
            explode.getFather().addValueOFNodeWithValueAtRight(explode, explode.getRightValue());
            explode.getFather().removeSon(explode);

            explode = this.getLeftmost(Node.EXPLODE_DEPTH);
        }

        return true;
    }

    public boolean split() {
        if (this.left_node == null) {
            if (this.left_value > 9) {
                this.left_node = new Node((byte) (this.left_value/2), (byte) Math.ceil(this.left_value/2.f));
                this.left_node.father = this;
                this.left_value = 0; // reinicia per si explota
                return true;
            }
        }
        else {
            if (this.left_node.split()) return true;
        }

        if (this.right_node == null) {
            if (this.right_value > 9) {
                this.right_node = new Node((byte) (this.right_value/2), (byte) Math.ceil(this.right_value/2.f));
                this.right_node.father = this;
                this.right_value = 0; // reinicia per si explota
                return true;
            }
        }
        else {
            if (this.right_node.split()) return true;
        }
        return false;
    }

    public Node add(Node n) {
        Node result = new Node(this, n);
        boolean change;
        do {
            change = result.explode();
            change |= result.split();
        } while (change);
        return result;
    }

    public int getMagnitude() {
        int r = 0;
        if (this.left_node != null) r += 3*this.left_node.getMagnitude();
        else r += 3*this.left_value;
        if (this.right_node != null) r += 2*this.right_node.getMagnitude();
        else r += 2*this.right_value;
        return r;
    }

    @Override
    public String toString() {
        String left = (this.left_node != null) ? this.left_node.toString() : String.valueOf(this.left_value),
                right = (this.right_node != null) ? this.right_node.toString() : String.valueOf(this.right_value);
        return "[" + left + "," + right + "]";
    }
}
