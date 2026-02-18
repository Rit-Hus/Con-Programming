package HW4.Task8;

public class Bathroom {

    private int menCount = 0;
    private int womenCount = 0;

    public synchronized void manEnter(int id) {
        System.out.println( " [Man " + id + "]: Waiting to enter the bathroom.");
        while (womenCount > 0) {
            try {
                wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        this.menCount++;
        System.out.println( " [Man " + id + "]: Entered. (Men inside: " + menCount + ")");
    }

    public synchronized void manExit(int id) {
        this.menCount--;
        System.out.println( " [Man " + id + "]: Exited. (Men inside: " + menCount + ")");
        if (this.menCount == 0) {
            notifyAll();
        }
    }

    public synchronized void womenEnter(int id) {
        System.out.println( " [Woman " + id + "]: Waiting to enter the bathroom.");
        while (menCount > 0) {
            try {
                wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        this.womenCount++;
        System.out.println( " [Woman " + id + "]: Entered. (Women inside: " + womenCount + ")");
    }

    public synchronized void womenExit(int id) {
        this.womenCount--;
        System.out.println( " [Woman " + id + "]: Exited. (Women inside: " + womenCount + ")");
        if (this.womenCount == 0) {
            notifyAll();
        }
    }

    
    public static void main(String[] args) {
        Bathroom bathroom = new Bathroom();

        for (int i = 0; i < 5; i++) {
            new Man(i, bathroom).start();
        }

        for (int i = 0; i < 5; i++) {
            new Woman(i, bathroom).start();
        }
    }
}