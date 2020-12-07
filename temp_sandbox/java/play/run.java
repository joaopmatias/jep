package play;

import jep.Interpreter;
import jep.SharedInterpreter;
import jep.JepException;
import jep.MainInterpreter;

public class run {
    public static void main(String[] args) {
        try {
            MainInterpreter.setSharedModulesArgv("t");
        } catch (JepException e) {

        }
        try (Interpreter interp = new SharedInterpreter()) {
            interp.eval("import run");
        } catch (JepException e) {

        }
        System.out.println("hi");
    }
}
