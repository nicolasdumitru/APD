package oneProducerOneConsumer;

public class Buffer {
    private int a;
    private boolean full = false;

    synchronized void put(int value) {
        while (full) {
            try {
                wait();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        a = value;
        full = true;
        notifyAll();
    }

    synchronized int get() {
        while (!full) {
            try {
                wait();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        full = false;
        notifyAll();
        return a;
    }
}
