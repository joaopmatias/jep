package play;

import java.nio.file.Path;

import jep.Interpreter;
import jep.JepConfig;
import jep.SharedInterpreter;
import jep.SubInterpreter;
import jep.JepException;
import jep.MainInterpreter;
import java.nio.file.Paths;

public class run {
    public synchronized static void main(String[] args) throws InterruptedException {
        JepConfig config = new JepConfig();
        config.addSharedModules("run_check_threads");


        try (Interpreter interp = new SharedInterpreter()) {
            System.out.println("\n   SharedInterpreter");
            interp.eval("import run");
            interp.eval("a = 1");
            // interp.eval("import run; run.run()");
        } catch (JepException e) {
            System.out.println(e.toString());
        }


        Thread thread = new Thread() {
            @Override
            public void run() {
                try (Interpreter interp = new SharedInterpreter()) {
                    System.out.println("\n   SharedInterpreter");
                    // interp.eval("import run");
                    // interp.eval("import run; run.run()");
                    interp.eval("import run");
                    wait();
                    interp.eval("run.run()");
                    wait();
                } catch (JepException | InterruptedException e) {
                    System.out.println(e);
                }
            }
        };
        thread.run();


        try (Interpreter interp = new SubInterpreter(config)) {
            // if one or more examples are not skipped it will throw an error due to shared resources
            System.out.println("\n   SubInterpreter");
            // interp.eval("import run");
            interp.eval("import run_check_threads");
        } catch (JepException e) {
            System.out.println(e);
        }
    }
}
