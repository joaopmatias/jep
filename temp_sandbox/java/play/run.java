package play;

import java.nio.file.Path;

import jep.Interpreter;
import jep.SharedInterpreter;
import jep.SubInterpreter;
import jep.JepException;
import jep.MainInterpreter;
import java.nio.file.Paths;

public class run {
    public static void main(String[] args) {
        try {
            MainInterpreter.setSharedModulesArgv("run");
        } catch (JepException e) {

        }
        try (Interpreter interp = new SharedInterpreter()) {
            interp.eval("");
            interp.eval("import t");
            interp.eval("print(sys.path)");
        } catch (JepException e) {
            System.out.println(e);
        }

        try (Interpreter interp = new SharedInterpreter()) {
            interp.eval("import sys");
            interp.eval("print(sys.path)");
        } catch (JepException e) {
            System.out.println(e);
        }
        System.out.println("hi");
    }
}
