static void main() {
    var p = Runtime.getRuntime().availableProcessors();

    Thread[] threads = new Thread[p];

    for (var i = 0; i < p; i += 1) {
        final int id = i;

        threads[i] = new Thread(() -> {
            System.out.println("Hello from thread #" + id);
        });
        threads[i].start();
    }

    for (var t : threads) {
        try {
            t.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
