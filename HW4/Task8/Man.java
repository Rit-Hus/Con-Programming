package HW4.Task8;

import java.util.Random;

class Man extends Thread {

    Bathroom bathroom;
    int id;

    public void run() {
        Random random = new Random();
        try {
            while (true) {
                this.bathroom.manEnter();
                Thread.sleep(random.nextInt(2000));
                this.bathroom.manExit();
                Thread.sleep(random.nextInt(4000));
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    public Man(int id, Bathroom bathroom) {
        this.id = id;
        this.bathroom = bathroom;
    }
}

