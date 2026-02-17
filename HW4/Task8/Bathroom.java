package HW4.Task8;

import java.util.Random;

public class Bathroom {

    private int menCount = 0;
    private int womenCount = 0;
    private boolean womenAccess = false;
    private boolean menAccess = false;

    public enum PrevGender {
        MEN,
        WOMEN
    }

    private PrevGender lastGender = PrevGender.MEN;

    public synchronized void manEnter() {
        while (womenCount > 0 && womenAccess) {
            try {
                wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                if (lastGender == PrevGender.MEN) {
                    try {
                        Thread.sleep(1);
                    } catch (InterruptedException ex) {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }
        this.menAccess = true;
        this.menCount++;
        System.out.println("man count = " + this.menCount + " women count = " + this.womenCount);
    }

    public synchronized void manExit() {
        this.menCount--;
        if (this.menCount == 0) {
            this.menAccess = false;
            lastGender = PrevGender.MEN; 
            notifyAll();
        }
        System.out.println("man count = " + this.menCount + " women count = " + this.womenCount);
    }

    public synchronized void womenEnter() {
        while (menCount > 0 && menAccess) {
            try {
                wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                if (lastGender == PrevGender.WOMEN) {
                    try {
                        Thread.sleep(1);
                    } catch (InterruptedException ex) {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }
        this.womenAccess = true;
        this.womenCount++;
        System.out.println("man count = " + this.menCount + " women count = " + this.womenCount);
    }

    public synchronized void womenExit() {
        this.womenCount--;
        if (this.womenCount == 0) {
            this.womenAccess = false;
            notifyAll();
            lastGender = PrevGender.WOMEN; 
        }
        System.out.println("man count = " + this.menCount + " women count = " + this.womenCount);
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

