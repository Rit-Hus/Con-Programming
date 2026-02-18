package HW4.Task8;

import java.util.Random;

class Man extends Thread {
    Bathroom bathroom;
    int id;

    public Man(int id, Bathroom bathroom) {
        this.id = id;
        this.bathroom = bathroom;
    }

    public void run() {
        Random random = new Random();
        try {
            while (true) {
                this.bathroom.manEnter(this.id);
                Thread.sleep(random.nextInt(2000));
                this.bathroom.manExit(this.id);
                Thread.sleep(random.nextInt(4000));
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}