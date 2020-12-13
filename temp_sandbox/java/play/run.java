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
            // interp.eval("import run");
            // interp.eval("import t");
            // interp.eval("print(sys.path)");
            interp.eval("a = 9");
        } catch (JepException e) {
            System.out.println(e);
        }

        try (Interpreter interp = new SharedInterpreter()) {
            // interp.eval("print(sys.path)");
            // interp.eval("print(sys.modules)");
            // interp.eval("import run");
            // interp.eval("print(a)");
        } catch (JepException e) {
            System.out.println(e);
        }
        System.out.println("hi");
    }
}
