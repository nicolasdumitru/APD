package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;
import java.util.concurrent.Semaphore;

public class Buffer {

    private final Queue<Integer> queue;
    private final Semaphore empty;
    private final Semaphore full;

    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
        empty = new Semaphore(size); // free slots
        full = new Semaphore(0); // filled slots
    }

    public void put(int value) {
        empty.acquireUninterruptibly();
        synchronized (this) {
            queue.add(value);
        }
        full.release();
    }

    public int get() {
        full.acquireUninterruptibly();
        int a = -1;
        synchronized (this) {
            a = queue.remove();
        }
        empty.release();
        return a;
    }
}
