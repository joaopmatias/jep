package play;

import jep.Interpreter;
import jep.JepConfig;
import jep.SharedInterpreter;
import jep.SubInterpreter;
import jep.JepException;

public class run {
    public synchronized static void main(String[] args) throws InterruptedException {
        JepConfig config = new JepConfig();
        config.addSharedModules("run");

        try (Interpreter interp = new SubInterpreter(config)) {
            System.out.println("\n   SubInterpreter");
            interp.eval("import run");
        } catch (JepException e) {
            System.out.println(e);
        }

        try (Interpreter interp = new SharedInterpreter()) {
            System.out.println("\n   SharedInterpreter");
            interp.eval("import run");
            interp.eval("run.run()");
        } catch (JepException e) {
            System.out.println(e.toString());
        }

        Thread thread = new Thread() {
            @Override
            public void run() {
                try (Interpreter interp = new SharedInterpreter()) {
                    System.out.println("\n   SharedInterpreter");
                    interp.eval("import run");
                    interp.eval("run.run()");
                } catch (JepException e) {
                    System.out.println(e);
                }
            }
        };
        thread.start();
        thread.join();


    }
}
