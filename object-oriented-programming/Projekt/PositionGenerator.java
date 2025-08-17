// Give it a starting position ant it will generate next positions baset on the array

public class PositionGenerator {
    // pc - current position
    // p0 - starting position
    int pc, pp, p0, i, k, cycle, xgap;
    int[] movement;
    boolean endStream;
    public int next(){
        
        if(endStream){
            return 0;
        }

        if(endofArray() || endofCycle()){
            edgeReached();
            return next();
        }

        pp = pc;
        pc += movement[i];
        k++;
        
        if(detectEdge()){
            endCycle();
            return next();
        }
        return pc;
    };
        
    public boolean END(){
        return endStream;
    }

    public int getStartingPos(){
        return p0;
    }

    public PositionGenerator(int p, int[] movement){
        i = 2;
        k = 0;
        cycle = movement[0];
        xgap = movement[1];
        this.movement = movement;
        pc = p;
        p0 = p;
        pp = p;
        endStream = false;
    }

    private boolean detectEdge(){
        if(pc > 63 || pc < 0){
            return true;
        }

        if(Math.abs((pc % 8) - (pp % 8)) > xgap){
            return true;
        }

        return false;
    }

    private boolean endofCycle(){
        return k >= cycle;
    }

    public void endCycle(){
        i++;
        k = 0;
        pc = p0;
    }

    private boolean endofArray(){
        return i >= movement.length;
    }

    private void edgeReached(){
        if(endofArray()){
            endStream = true;
        } else if(k >= cycle){
            k = 0;
            i++;
            pc = p0;
        }
    }
}
