public class Player {
    String name, side;
    char[] taken;

    public Player(String name, String side){
        taken = new char[16];
        this.name = name;
        this.side = side;
    }
}
