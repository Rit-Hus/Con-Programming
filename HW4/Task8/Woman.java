package HW4.Task8;

import java.util.Random;

class Woman extends Thread {

    Bathroom bathroom;
    int id;

    public void run() {
        Random random = new Random();
        try {
            while (true) {
                this.bathroom.womenEnter();
                Thread.sleep(random.nextInt(2000));
                this.bathroom.womenExit();
                Thread.sleep(random.nextInt(2000));
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    public Woman(int id, Bathroom bathroom) {
        this.id = id;
        this.bathroom = bathroom;
    }
    
}
