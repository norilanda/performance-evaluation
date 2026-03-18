public class FloatingPointInstructions extends Workload {
    public static volatile double VAR1 = 42;
    public static volatile double VAR2 = 1;
    public static volatile double VAR3 = 2;
    public static volatile double VAR4 = 3;
    public static volatile double VAR5 = 4;
    public static volatile double VAR6 = 5;
    
    @Override
    public int execute () {
        return (int) (VAR1 + VAR2 + VAR3 + VAR4 + VAR5 + VAR6);
    }
}
