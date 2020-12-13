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
            MainInterpreter.setSharedModulesArgv();
        } catch (JepException e) {
            System.out.println(e);
        }

        try (Interpreter interp = new SharedInterpreter()) {
            System.out.println("\n   SharedInterpreter");
            // interp.eval("import run");
            interp.eval("import run_check_threads");
        } catch (JepException e) {
            System.out.println(e);
        }

        try (Interpreter interp = new SubInterpreter()) {
            // if one or more examples are not skipped it will throw an error due to shared resources
            System.out.println("\n   SubInterpreter");
            // interp.eval("import run");
            interp.eval("import run_check_threads");
        } catch (JepException e) {
            System.out.println(e);
        }
    }
}
